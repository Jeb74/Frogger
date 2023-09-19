#include "../headers/clock.h"

/**
 * Gestisce il timer del gioco.
 * @param arg   Gli argomenti del thread.
 */
void *manage_clock(void *arg)
{
    ExecutionMode exm = get_exm();

    GameArgs *args;
    unpack(arg, args, get_exm(), GAMEPKG);

    Board *board = args->board;
    board->time_left = board->max_time;

    while (true)
    {
        SLEEP_SECONDS(CLOCK_HIT_EVERY);
        board->time_left -= board->time_left > 0 ? CLOCK_HIT_EVERY : 0;

        if (exm == PROCESS)
        {
            // TODO - Mandare i pacchetti di time via pipe.
        }
        else
        {
            // TODO - Mandare i pacchetti di time via thread.
        }
    }
}

/**
 * Formatta il timer del gioco.
 * @param board La tabella di gioco.
 * @return      Il timer formattato (NECESSITA DI FREE DOPO L'USE).
 */
char *format_clock_numeric(Board *board) {
    int time_left = board->time_left;

    char* minutes = num_to_string((int)(time_left / 60), 2, true);
    char* seconds = num_to_string((int)(time_left % 60), 2, true);

    char *clock_fmt = build_string("%s:%s", minutes, seconds);
    free(minutes);
    free(seconds); 

    return clock_fmt;
}

/**
 * Formatta il timer del gioco in una barra.
 * @param board La tabella di gioco.
 * @return      Il timer formattato (NECESSITA DI FREE DOPO L'USE).
 */
char *format_clock_bar(Board *board)
{
    char *clock = MALLOC_TERM(char, ALLOC_SIZE_TIME_LEFT_BAR);
    CRASH_IF_NULL(clock);

    int seconds = board->time_left;
    int max_time = board->max_time;

    int true_size = ALLOC_SIZE_TIME_LEFT_BAR - TERM;
    int left = true_size - ALREADY_WRITTEN_CHARS;

    int seconds_per_char = max_time / left;
    int chars = seconds / seconds_per_char;

    snprintf(clock, ALLOC_SIZE_TIME_LEFT_BAR, "TIME LEFT: [           ]");

    for (int i = 1; i < true_size; i++)
    {
        if (i <= chars)
        {
            clock[i + WRITTEN_TIME_CHARS] = TIME_LEFT_CHAR;
        }
    }

    return clock;
}