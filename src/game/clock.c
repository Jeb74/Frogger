#include "../headers/clock.h"

/**
 * Gestisce il timer del gioco.
 * @param arg   Gli argomenti del thread.
 */
void *manage_clock(void *arg)
{
    GameArgs *args;
    unpack(arg, args, THREAD, GAMEPKG);

    Board *board = args->board;

    board->time_left = board->max_time;

    while (true)
    {
        SLEEP_SECONDS(CLOCK_HIT_EVERY);
        board->time_left -= board->time_left > 0 ? CLOCK_HIT_EVERY : 0;
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

/**
 * Formatta il timer del gioco in una barra.
 * @param board La tabella di gioco.
 * @return      Il timer formattato (NECESSITA DI FREE DOPO L'USE).
*/
char *format_clock_bar(Board *board)
{
    char *clock = MALLOC(char, MSG_SIZE_TIME_LEFT_BAR);
    CRASH_IF_NULL(clock);

    int seconds = board->time_left;
    int max_time = board->max_time;

    int left = MSG_SIZE_TIME_LEFT_BAR - 13;
    int seconds_per_line = max_time / left;
    int lines = seconds / seconds_per_line;

    snprintf(clock, MSG_SIZE_TIME_LEFT_BAR, "TIME LEFT: [           ]");

    for (int i = 1; i < MSG_SIZE_TIME_LEFT_BAR; i++)
    {
        if (i <= lines)
        {
            clock[i + 11] = TIME_LEFT_CHAR;
        }
    }

    return clock;
}