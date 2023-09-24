#include "../../headers/threading.h"

/**
 * Crea un thread.
 * @param thread    Il thread da creare.
 * @param manager   Il manager del thread.
 * @param args      Gli argomenti del thread.
 */
void create_thread(pthread_t *thread, void *manager, Package *args)
{
    pthread_create(thread, NULL, manager, args);
}

/**
 * Crea un array di thread.
 * @param threads       L'array di thread da creare.
 * @param num_threads   Il numero di thread da creare.
 * @param manager       Il manager dei thread.
 * @param args          Gli argomenti dei thread.
 */
void create_threads(pthread_t **threads, int num_threads, void *manager, Package *args)
{
    *threads = MALLOC(pthread_t, num_threads);

    for (int i = 0; i < num_threads; i++)
    {
        create_thread(&(*threads)[i], manager, args);
    }
}

/**
 * Aspetta che un thread termini.
 * @param thread    Il thread da aspettare.
 */
void join_thread(pthread_t *thread)
{
    pthread_join(*thread, NULL);
}

/**
 * Distrugge un thread.
 * @param thread    Il thread da distruggere.
 */
int cancel_thread(pthread_t *thread)
{
    return pthread_cancel(*thread);
}

/**
 * Distrugge un array di thread.
 * @param threads       L'array di thread da distruggere.
 * @param num_threads   Il numero di thread da distruggere.
 */
int *cancel_threads(pthread_t *threads, int num_threads)
{
    int *codes = MALLOC(int, num_threads);

    for (int i = 0; i < num_threads; i++)
    {
        codes[i] = pthread_cancel(threads[i]);
    }

    return codes;
}

void clone_update_life(Board *board, Board *boardClone, pthread_mutex_t *mutex)
{
    EXEC_WHILE_LOCKED(mutex, boardClone->lifes_left = board->lifes_left)
}

void clone_update_time(Board *board, Board *boardClone, pthread_mutex_t *mutex)
{
    EXEC_WHILE_LOCKED(mutex, boardClone->time_left = board->time_left)
}

void clone_update_score(Board *board, Board *boardClone, pthread_mutex_t *mutex)
{
    EXEC_WHILE_LOCKED(mutex, boardClone->points = board->points)
}

void clone_frog_position(Board *board, Board *boardClone)
{
    boardClone->fp = board->fp;
}

LOWCOST_INFO execute_action_on(Position *p, Action a, EntityTypes t) {
    switch(a) {
        case LEFT:
            p->x -= 1;
            break;
        case RIGHT:
            p->x += 1;
            break;
        case UP: {
                if (t == FROG) p->y -= 2;
                else p->y -= 1;
            }
            break;
        case DOWN: {
                if (t == FROG) p->y += 2;
                else p->y += 1;
            }
            break;
        case SHOOT: {
                if (t == FROG) return 1;
                else return 2;
            }
            break;
        case RQPAUSE: {
                hopper(true);
            }
            break;
        case RQQUIT: return KILL_SIGNAL;
        default:
            break;
    }
    return 0;
}

LOWCOST_INFO fetch_entities(
    Board *board, 
    struct ActionData *ra, 
    unsigned int *counter, 
    pthread_mutex_t *mutex, 
    EntityQueue *eq
    )
{
    bool shooting = false;
    Action action;

    AQUIRE_LOCK(mutex);
    
    for (int i = 0, curr_id = 0; i < *counter; i++) {
        curr_id = ra[i].id;
        Entity *e;
        Position *p;
        EntityTypes t;
        if (curr_id != 127) {
            e = walk_through(eq, ra[i].id);
            p = &e->position;
            t = e->type;
        }
        else {
            p = &board->fp;
            t = FROG;
        }
        execute_action_on(p, ra[i].action, t);
    }
    *counter = 0;

    RELEASE_LOCK(mutex);
}

LOWCOST_INFO thread_mode_exec(Screen screen)
{
    erase();
    refresh();

    Board board, boardClone;

    INIT_COLORS;
    INIT_BOARD(board, screen);
    INIT_BOARD(boardClone, screen);
    WINDOW **ws = create_windows(screen);

    ExecutionMode exm = get_exm();

    /* Clock start */

    pthread_t clockThread;
    pthread_mutex_t clockMutex = PTHREAD_MUTEX_INITIALIZER;

    Package *clockPackage = pack(exm, CLOCK_PKG, &clockMutex, &(board.time_left));

    create_thread(&clockThread, manage_clock, clockPackage);

    /* Clock end */
    
    /* Other entities start */

    pthread_t *entityThreads = MALLOC(pthread_t, STD_ENTITIES);
    pthread_mutex_t entityMutex = PTHREAD_MUTEX_INITIALIZER;

    EntityQueue *queue = create_queue(board);
    struct ActionData *wbuffer = MALLOC(struct ActionData, STD_ENTITIES*2);
    int counter = 0;

    Package **packages = MALLOC(Package *, STD_ENTITIES);

    for (size_t i = 0; i < STD_ENTITIES; i++)
    {
        Entity *entity = walk_through(queue, i);
        packages[i] = pack(exm, ENTITY_PKG, &entityMutex, &wbuffer, &counter, entity->id, &entity->action);
        create_thread(&entityThreads[i], manage_entity_movement, packages[i]);
    }

    /* Frog start */

    pthread_t frogThread;

    Package *frogPackage = pack(exm, FROG_PKG, &entityMutex, &wbuffer, &counter, 127);

    create_thread(&frogThread, manage_frog, frogPackage);

    /* Frog end */

    do
    {
        fetch_entities(&board, wbuffer, &counter, &entityMutex, queue);
        // TODO all movements

        //check_collisions(&board, queue);

        clone_update_life(&board, &boardClone, &clockMutex);
        clone_update_time(&board, &boardClone, &clockMutex);
        clone_update_score(&board, &boardClone, &clockMutex);
        clone_frog_position(&board, &boardClone);

        update_graphics(&boardClone, queue, ws);

        block(true);
        hopper(false);
    } while (true);

    destroy_package(clockPackage);

    return 2;
}