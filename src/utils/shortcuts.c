#include "../headers/shortcuts.h"

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

LOWCOST_INFO getAction(int argc, char *argv[]) {
    LOWCOST_INFO rslt = 0;
    if (argc > 3) {
        perror("Too many arguments: ./frogger <OPTIONAL:execution_type> <OPTIONAL:quit_after_execution>\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < argc; i++) {
        if (strcmp("-p", argv[i]) == 0) rslt += 1;
        else if (strcmp("-t", argv[i]) == 0) rslt += 0; 
        else if (strcmp("-q", argv[i]) == 0) rslt += 3; 
    }
    if (rslt > 4) {
        perror("Wrong arguments: ./frogger <OPTIONAL:execution_type> <OPTIONAL:quit_after_execution>\n");
        exit(EXIT_FAILURE);
    }
    return rslt;
} 

unsigned int calwidth(unsigned int width, unsigned int panels, LOWCOST_INFO indx) {
    int defsize = width / panels;
    if (width != panels*defsize) {
        int leftovers = width % panels;
        if (panels % 2 == 0)
            if (indx == panels / 2 + 1 || indx == panels / 2)
                return defsize + leftovers / 2;
        else if (indx == panels / 2 + 1)
            return defsize + leftovers;
        return defsize;
    }
}
