#include "../../headers/threading.h"

/**
 * Crea un thread.
 * @param thread    Il thread da creare.
 * @param manager   Il manager del thread.
 * @param args      Gli argomenti del thread.
 */
void create_thread(pthread_t *thread, void *manager, Package args)
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
void create_threads(pthread_t *threads, int num_threads, void *manager, Package args)
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
    int* codes = (int*) calloc(sizeof(int), num_threads);
    for (int i = 0; i < num_threads; i++) 
    {
        codes[i] = pthread_cancel(threads[i]);
    }
    return codes;
}

/**
 * Distrugge il mutex.
 */
void kill_mutex()
{
    pthread_mutex_destroy(&MUTEX);
}