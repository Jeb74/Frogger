#include "../headers/clock.h"

/**
 * Gestisce il timer del gioco.
 * @param arg   Gli argomenti del thread.
 */
void manage_clock(void *args)
{
    ExecutionMode exm = get_exm();
    bool process_mode = exm == PROCESS;

    void *data;

    unpack(args, data, exm, GENPKG);

    pipe_t w, r;
    unsigned int *time;
    if (process_mode) {
        w = findpn(2, data, "readtime");
        r = findpn(2, data, "writetime");
        CLOSE_READ(w);
        CLOSE_WRITE(r);
        time = CALLOC(unsigned int, 1);
        readfrm(time, r, sizeof(unsigned int));
    }
    else time = (unsigned int*) data;

    while (time != 0)
    {
        SLEEP_SECONDS(CLOCK_HIT_EVERY);
        *time -= *time > 0 ? CLOCK_HIT_EVERY : 0;

        if (process_mode)
        {
            LOWCOST_INFO info;
            if (readifready(&info, r, sizeof(LOWCOST_INFO))) exit(EXIT_SUCCESS);
            writeto(time, w, sizeof(unsigned int));
        }
    }
    if (process_mode) {
        CLOSE_READ(r);
        CLOSE_WRITE(w);
        exit(EXIT_SUCCESS);
    }
    return;
}

/**
 * Formatta il timer del gioco.
 * @param board La tabella di gioco.
 * @return      Il timer formattato (NECESSITA DI FREE DOPO L'USE).
 */
char *format_clock_numeric(Board *board) 
{
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