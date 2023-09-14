#include "../headers/clock.h"

/**
 * Gestisce il timer del gioco.
 * @param arg   Gli argomenti del thread.
 */
void *manage_clock(void *arg)
{
    GameArgs *args = (GameArgs *) arg;
    Board *board = args->board;

    while (true)
    {
        SLEEP_SECONDS(CLOCK_HIT_EVERY);
        board->time_left -= CLOCK_HIT_EVERY;
    }
}

/**
 * Formatta il timer del gioco.
 * @param board La tabella di gioco.
 * @return      Il timer formattato (NECESSITA DI FREE DOPO L'USE).
*/
char *format_clock(Board *board)
{
    char *clock = MALLOC(char, MSG_SIZE_TIME_LEFT);
    CRASH_IF_NULL(clock);

    int minutes = (int) (board->time_left / 60);
    int seconds = (int) (board->time_left % 60);

    snprintf(clock, MSG_SIZE_TIME_LEFT, "TIME LEFT: %02i:%02i", minutes, seconds);

    return clock;
}