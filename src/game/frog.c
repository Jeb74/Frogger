#include "../headers/frog.h"

bool get_keyboard_action(Action *action)
{
    switch (getch())
    {
        case 'a':
            *action = LEFT;
            break;
        case 'd':
            *action = RIGHT;
            break;
        case 'w':
            *action = UP;
            break;
        case 's':
            *action = DOWN;
            break;
        case ' ':
            *action = SHOOT;
            break;
        case KEY_ESC:
            *action = RQPAUSE;
            break;
        case 'q':
            *action = RQQUIT;
            break;
        default:
            return false;
    }

    return true;
}

/**
 * Gestisce i movimenti della rana.
 * @param arg   Gli argomenti del thread.
 */
void *manage_frog(void *args)
{
    ExecutionMode exm = get_exm();
    bool process_mode = exm == PROCESS;

    TinyEntityMovePacket *data;
    unpack(args, data, exm, FROG_PKG);

    Action action = NONE;

    struct timeval st, ct;
    gettimeofday(&st, NULL);

    long udelay, delay;

    LOWCOST_INFO signal = 0;
    bool ready = true;

    if (process_mode)
    {
        CLOSE_READ(data->carriage.p.c);
    }

    while (true)
    {
        if (!get_keyboard_action(&action))
        {
            continue;
        }

        gettimeofday(&ct, NULL);

        delay = ct.tv_sec - st.tv_sec;
        udelay = ct.tv_usec - st.tv_usec;


        if (udelay >= 100000 || delay >= 1)
        {
            if (process_mode)
            {
                writeto(&action, data->carriage.p.c, sizeof(Action));
                writeto(&ready, data->carriage.p.s, sizeof(bool));
                st = ct;
            }
            else
            {
                struct ActionData adata = {
                    .action = action,
                    .id = data->id,
                };

                EXEC_WHILE_LOCKED(data->carriage.t.entity_mutex,
                                  {
                                      int *count = data->carriage.t.counter;
                                      (*count)++;

                                      data->carriage.t.wbuffer[*count] = adata;
                                  })
                block(false);
            }
        }
        
        if (process_mode)
        {
            signal = action;
            if (signal == RQQUIT)
            {
                break;
            }
            else if (signal == RQPAUSE)
            {
                readfrm(&signal, data->carriage.p.se, sizeof(LOWCOST_INFO));
            }
        }
        else
        {
            hopper(false);
        }
    }

    if (process_mode) 
    {
        CLOSE_WRITE(data->carriage.p.c);
        CLOSE_READ(data->carriage.p.s);
        CLOSE_READ(data->carriage.p.se);
    }


    return;
}

/**
 * Formatta le vite della rimanenti della rana.
 * @param board La board.
 * @return      Le vite formattate (NECESSITA DI FREE DOPO L'USE).
 */
char *format_lives_numeric(Board *board)
{
    char *lifes = num_to_string(board->lifes_left, count_digits(board->lifes_left));
    CRASH_IF_NULL(lifes);

    char *lifes_fmt = build_string("%s \xE2\x99\xA5", lifes);
    CRASH_IF_NULL(lifes_fmt);

    free(lifes);

    return lifes_fmt;
}
