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

void clone_update_life(Board *board, Board *boardClone, pthread_mutex_t mutex)
{
    EXEC_WHILE_LOCKED(mutex, boardClone->lifes_left = board->lifes_left)
}

void clone_update_time(Board *board, Board *boardClone, pthread_mutex_t mutex)
{
    EXEC_WHILE_LOCKED(mutex, boardClone->time_left = board->time_left)
}

void clone_update_score(Board *board, Board *boardClone, pthread_mutex_t mutex)
{
    EXEC_WHILE_LOCKED(mutex, boardClone->points = board->points)
}

LOWCOST_INFO thread_mode_exec(Screen screen) 
{
    erase();
    refresh();
    
    Board board, boardClone;

    INIT_COLORS;
    INIT_BOARD(board, screen);
    INIT_BOARD(boardClone, screen);

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

    Package *frogPackage = pack(exm, FROG_PKG, &frogMutex, &board);

    create_thread(&frogThread, manage_frog, frogPackage);

    /* Frog end */

    do
    {
        clone_update_life(&board, &boardClone, clockMutex);
        clone_update_time(&board, &boardClone, clockMutex);
        clone_update_score(&board, &boardClone, clockMutex);

        update_graphics(&boardClone, NULL);
        SLEEP_MILLIS(100);
    } while (true);

    destroy_package(clockPackage);

    return 2;
}