#include "../headers/frog.h"

/**
 * Gestisce i movimenti della rana.
 * @param arg   Gli argomenti del thread.
 */
void *manage_frog(void *args)
{
    ExecutionMode exm = get_exm();
    bool process_mode = exm == PROCESS;

    void *data;
    unpack(args, data, exm, GENPKG);

    Action action = NONE;
    pipe_t w, r, s;
    if (process_mode) {
        w = findpn(PAS, data, "readaction");
        r = findpn(PAS, data, "writeaction");
        s = findpn(PAS, data, "readysignal");
        CLOSE_READ(w);
        CLOSE_WRITE(r);
        CLOSE_READ(s);
    }

    struct timeval st, ct;
    gettimeofday(&st, NULL);
    while (true)
    {
        switch (wgetch(stdscr))
        {
            case KEY_LEFT:
            case 'a':
                action = LEFT;
                break;
            case KEY_RIGHT:
            case 'd':
                action = RIGHT;
                break;
            case KEY_UP:
            case 'w':
                action = UP;
                break;
            case KEY_DOWN:
            case 's':
                action = DOWN;
                break;
            case ' ':
                action = SHOOT;
                break;
            default:
                continue;
        }

        if (exm == PROCESS)
        {
            LOWCOST_INFO info;
            if (readifready(&info, r, sizeof(LOWCOST_INFO))) exit(EXIT_SUCCESS);
            gettimeofday(&ct, NULL);
            if ((ct.tv_usec - st.tv_usec) >= 100000 || (ct.tv_sec - st.tv_sec) >= 1) {
                writeto(&action, w, sizeof(Action));
                writeto(&info, s, sizeof(bool));
                st = ct;
            }
        }
        else
        {
            // TODO - Mandare i pacchetti di movimento via thread.
        }
    }
    if (process_mode) {
        CLOSE_READ(r);
        CLOSE_WRITE(w);
        exit(EXIT_SUCCESS);
    }
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
