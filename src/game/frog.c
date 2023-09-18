#include "../headers/frog.h"

/**
 * Gestisce i movimenti della rana.
 * @param arg   Gli argomenti del thread.
 */
void *manage_frog(void *arg)
{
    GameArgs *args = (GameArgs *)arg;

    while (true)
    {
        
    }
}

/**
 * Formatta le vite della rimanenti della rana.
 * @param board La board.
 * @return      Le vite formattate (NECESSITA DI FREE DOPO L'USE).
 */
char *format_lives_numeric(Board *board)
{
    char *lifes = numToString(board->lifes_left, -1, false);
    CRASH_IF_NULL(lifes);
    char *lifes_fmt = build_string("%s \xE2\x99\xA5", lifes);
    CRASH_IF_NULL(lifes_fmt);
    free(lifes);

    return lifes_fmt;
}
