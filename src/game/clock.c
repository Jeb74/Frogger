#include "../headers/clock.h"

/**
 * Gestisce il timer del gioco.
 * @param arg   Gli argomenti del thread.
 */
void *manage_clock(void *arg)
{
    ThreadArgs *args = (ThreadArgs *) arg;
    Board *board = args->board;

    while (true)
    {
        SLEEP_SECONDS(CLOCK_HIT_EVERY);

        pthread_mutex_lock(&MUTEX);

        board->time_left--;

        printf("time left while running: %d\n", board->time_left);
        pthread_mutex_unlock(&MUTEX);
    }
}