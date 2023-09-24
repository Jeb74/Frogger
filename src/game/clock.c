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

    LOWCOST_INFO signal;

    if (process_mode)
    {
        data->time_left = CALLOC(unsigned int, 1);
        readfrm(data->time_left, data->carriage.p.c, sizeof(unsigned int));
        CLOSE_READ(data->carriage.p.c);
        CLOSE_READ(data->carriage.p.s);
        CLOSE_WRITE(data->carriage.p.se);
    }

    while (!data->cancelled)
    {
        SLEEP_SECONDS(CLOCK_HIT_EVERY);
        EXEC_WHILE_LOCKED_IF_THREAD(data->carriage.t.mutex, *data->time_left -= *data->time_left > 0 ? CLOCK_HIT_EVERY : 0)

        if (process_mode)
        {
            signal = !data->cancelled;
            readifready(&signal, data->carriage.p.se, sizeof(LOWCOST_INFO));

            if (signal == KILL_SIGNAL)
            {
                data->cancelled = true;
            }
            else if (signal == PAUSE_SIGNAL)
            {
                readfrm(&signal, data->carriage.p.se, sizeof(LOWCOST_INFO));
                *data->time_left += 1;
            }
            else
            {
                writeto(data->time_left, data->carriage.p.c, sizeof(unsigned int));
                writeto(&signal, data->carriage.p.s, sizeof(bool));
            }
        }
        else
        {
            block(false);
            hopper(false);
        }

        EXEC_WHILE_LOCKED_IF_THREAD(data->carriage.t.mutex, data->cancelled = *data->time_left <= 0)
    }

    if (process_mode)
    {
        CLOSE_WRITE(data->carriage.p.s);
        CLOSE_WRITE(data->carriage.p.c);
        CLOSE_READ(data->carriage.p.se);
        free(data->time_left);
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
    char *seconds = num_to_string((time_left % 60), 2);

    char *clock_fmt = build_string("%s:%s", minutes, seconds);
    free(minutes);
    free(seconds);

    return clock_fmt;
}
