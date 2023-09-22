#include "../headers/clock.h"

/**
 * Gestisce il timer del gioco.
 * @param arg   Gli argomenti del thread.
 */
void *manage_clock(void *args)
{
    ExecutionMode exm = get_exm();
    bool process_mode = exm == PROCESS;

    ClockPacket *data;
    unpack(args, data, exm, CLOCK_PKG);

    if (process_mode)
    {
        CLOSE_READ(data->carriage.p.w);
        CLOSE_READ(data->carriage.p.s);
        CLOSE_WRITE(data->carriage.p.r);

        data->time_left = CALLOC(unsigned int, 1);
        readfrm(data->time_left, data->carriage.p.r, sizeof(unsigned int));
    }

    while (!data->cancelled)
    {
        SLEEP_SECONDS(CLOCK_HIT_EVERY);
        *data->time_left -= *data->time_left > 0 ? CLOCK_HIT_EVERY : 0;

        if (process_mode)
        {
            LOWCOST_INFO ongoing = !data->cancelled;

            if (readifready(&ongoing, data->carriage.p.s, sizeof(LOWCOST_INFO)) && ongoing == KILL_SIGNAL)
            {
                data->cancelled = true;
            }
            else
            {
                writeto(data->time_left, data->carriage.p.w, sizeof(unsigned int));
                writeto(&ongoing, data->carriage.p.s, sizeof(bool));
            }
        }

        if (*data->time_left <= 0)
        {
            data->cancelled = true;
        }
    }
    
    if (process_mode)
    {
        CLOSE_READ(data->carriage.p.r);
        CLOSE_WRITE(data->carriage.p.w);
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

    char *minutes = num_to_string((int)(time_left / 60), 2);
    char *seconds = num_to_string((int)(time_left % 60), 2);

    char *clock_fmt = build_string("%s:%s", minutes, seconds);
    free(minutes);
    free(seconds);

    return clock_fmt;
}
