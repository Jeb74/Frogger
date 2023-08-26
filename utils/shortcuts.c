#include "shortcuts.h"

/**
 * Genera un numero casuale in un determinato range X->Y (inclusi).
 * @param min   Estremo minore.
 * @param max   Estremo maggiore.
 * @return      Il numero casuale generato.
 */
int gen_num(int min, int max)
{
    return min + rand() % (max - min + 1);
}

/**
 * Termina il gioco.
 * @param board Il puntatore alla board.
 */
void end_game(Board *board)
{
    pthread_mutex_destroy(&MUTEX);
    endwin();
}

/**
 * Trova il simbolo corrispondente ad un determinato tipo di entità.
 * @param type  Il tipo di entità.
 * @return      Il simbolo corrispondente.
 */
char get_entity_symbol(EntityTypes type)
{
    switch (type)
    {
        case FROG:
            return 'F';
        case CAR:
            return 'C';
        case TRUCK:
            return 'T';
        case LOG:
            return 'L';
        case ENEMY_FROG:
            return 'E';
        case ENEMY_BIRD:
            return 'B';
        case ENEMY_SNAKE:
            return 'S';
        case PROJECTILE:
            return ':';
        default:
            return '?';
    }
}

/**
 * Prende il valore della direzione.
 * @param direction La direzione.
 * @return          Il valore della direzione.
 */
int get_direction_value(Direction direction)
{
    switch (direction)
    {
        case LEFT:
            return -1;
        case RIGHT:
            return 1;
        case UP:
            return -1;
        case DOWN:
            return 1;
        default:
            return 0;
    }
}