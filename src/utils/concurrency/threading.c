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

void clone_frog_position(Board *board, Board *boardClone, pthread_mutex_t *mutex)
{
    EXEC_WHILE_LOCKED(mutex, boardClone->fp = board->fp)
}

bool fetch_frog_t(Board *board, Action *receivedAction, pthread_mutex_t *mutex)
{
    bool shooting = false;
    Action action;

    EXEC_WHILE_LOCKED(mutex, action = *receivedAction)

    if (action == RQPAUSE)
    {
        hopper(true);
    }
    else
    {
        board->fp.x += action == RIGHT ? 1 : action == LEFT ? -1 : 0;
        board->fp.y += action == UP ? -2 : action == DOWN ? 2 : 0;
        shooting = action == SHOOT;
    }

    return shooting;
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

    /* Frog start */

    pthread_t frogThread;
    pthread_mutex_t frogMutex = PTHREAD_MUTEX_INITIALIZER;
    Action frogAction = NONE;

    Package *frogPackage = pack(exm, FROG_PKG, &frogMutex, &frogAction);

    create_thread(&frogThread, manage_frog, frogPackage);

    /* Frog end */
    
    /* Other entities start */

    pthread_t *entityThreads;
    pthread_mutex_t *entityMutexes;

    EntityQueue *queue = create_queue(board);
    Action *entityActions = MALLOC(Action, STD_ENTITIES);

    create_threads(&entityThreads, STD_ENTITIES, manage_entity_movement, pack(exm, ENTITY_PKG, &entityMutexes, &entityActions));

    do
    {
        fetch_frog_t(&board, &frogAction, &frogMutex);
        // TODO all movements

        check_collisions(&board, NULL);

        clone_update_life(&board, &boardClone, &clockMutex);
        clone_update_time(&board, &boardClone, &clockMutex);
        clone_update_score(&board, &boardClone, &clockMutex);
        clone_frog_position(&board, &boardClone, &frogMutex);

        update_graphics(&boardClone, NULL);

        frogAction = NONE;

        block(true);
        hopper(false);
    } while (true);

    destroy_package(clockPackage);

    return 2;
}