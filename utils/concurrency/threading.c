#include "threading.h"

/**
 * Crea un thread.
 * @param thread    Il thread da creare.
 * @param manager   Il manager del thread.
 * @param args      Gli argomenti del thread.
 */
void create_thread(pthread_t *thread, void *manager, ThreadArgs *args)
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
void create_threads(pthread_t *threads, int num_threads, void *manager, ThreadArgs *args)
{
    for (int i = 0; i < num_threads; i++)
    {
        create_thread(&(threads[i]), manager, args);
    }
}

/**
 * Distrugge un thread.
 * @param thread    Il thread da distruggere.
 */
void cancel_thread(pthread_t *thread)
{
    pthread_cancel(*thread);
}

/**
 * Distrugge un array di thread.
 * @param threads       L'array di thread da distruggere.
 * @param num_threads   Il numero di thread da distruggere.
 */
void cancel_threads(pthread_t *threads, int num_threads) 
{
    for (int i = 0; i < num_threads; i++) 
    {
        pthread_cancel(threads[i]);
    }
}

/**
 * Distrugge il mutex.
 */
void kill_mutex()
{
    pthread_mutex_destroy(&MUTEX);
}