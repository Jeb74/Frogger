#include "../headers/frog.h"

/**
 * Gestisce i movimenti della rana.
 * @param arg   Gli argomenti del thread.
 */
void *manage_frog(void *arg)
{
    GameArgs *args = (GameArgs *)arg;

    Direction direction;

    while (true)
    {
        switch (getch())
        {
        case KEY_LEFT:
            direction = LEFT;
            break;
        case KEY_RIGHT:
            direction = RIGHT;
            break;
        case KEY_UP:
            direction = UP;
            break;
        case KEY_DOWN:
            direction = DOWN;
            break;
        default:
            continue;
        }

        /*
        TODO - Mandare i pacchetti di movimento.
            -> collisioni
            -> print screen
        */
    }
}

/**
 * Formatta le vite della rimanenti della rana.
 * @param board La board.
 * @return      Le vite formattate (NECESSITA DI FREE DOPO L'USE).
 */
char *format_lives_numeric(Board *board)
{
    char *lifes = num_to_string(board->lifes_left, -1, false);
    CRASH_IF_NULL(lifes);
    char *lifes_fmt = build_string("%s \xE2\x99\xA5", lifes);
    CRASH_IF_NULL(lifes_fmt);
    free(lifes);

    return lifes_fmt;
}
