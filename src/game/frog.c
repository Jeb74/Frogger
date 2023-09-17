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
    int alloc_size = count_digits(board->lifes_left) + TERM;

    char *lives = MALLOC(char, alloc_size);
    CRASH_IF_NULL(lives);

    snprintf(lives, alloc_size, "%i", board->lifes_left);

    return lives;
}
