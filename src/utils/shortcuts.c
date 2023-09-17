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
/**
 * Conta il numero di cifre di un numero.
 * @param value Il numero.
*/
int count_digits(int value)
{
    int digits = 0;

    while (value > 0)
    {
        value /= 10;
        ++digits;
    }

    return digits;
}

/**
 * Divide due numeri se possibile altrimenti ritorna il dividendo.
 * @param dividend  Il dividendo.
 * @param divisor   Il divisore
*/
int divide_if_possible(int dividend, int divisor)
{
    if (divisor == 0 || dividend == 0)
    {
        return 0;
    }

    return dividend >= divisor ? dividend / divisor : dividend;
}

/**
 * Calcola la percentuale.
 * @param dividend  Il dividendo.
 * @param divisor   Il divisore.
*/
double calculate_percentage(int dividend, int divisor)
{
    if (divisor == 0 || dividend == 0)
    {
        return 0;
    }

    return ((double) dividend / divisor) * 100;
}

/**
 * Crea una barra.
 * @param board     Il puntatore alla board.
 * @param size      La dimensione della barra.
 * @param max       Il massimo.
 * @param current   Il valore corrente.
 * @return          La barra.
*/
Bar create_bar(Board *board, size_t size, int max, int current)
{
    Bar bar;

    double percentage = calculate_percentage(current, max);

    bar.percent_full = percentage;
    bar.percent_empty = 100 - percentage;

    return bar;
}

/**
 * Crea una barra della vita.
 * @param board Il puntatore alla board.
 * @param size  La dimensione della barra.
 * @return      La barra.
*/
Bar create_life_bar(Board *board, size_t size)
{
    return create_bar(board, size, board->lifes_on_start, board->lifes_left);
}

/**
 * Crea una barra del tempo.
 * @param board Il puntatore alla board.
 * @param size  La dimensione della barra.
 * @return      La barra.
*/
Bar create_time_bar(Board *board, size_t size)
{
    return create_bar(board, size, board->max_time, board->time_left);
}

/**
 * Aggiorna la barra.
 * @param bar       La barra.
 * @param max       Il massimo.
 * @param current   Il valore corrente.
*/
void calculate_bar(Bar *bar, int max, int current)
{
    double percentage = calculate_percentage(current, max);

    bar->percent_full = percentage;
    bar->percent_empty = 100 - percentage;
}

/**
 * Aggiorna la barra della vita.
 * @param bar       La barra.
 * @param board     Il puntatore alla board.
*/
void calculate_life_bar(Bar *bar, Board *board)
{
    calculate_bar(bar, board->lifes_on_start, board->lifes_left);
}

/**
 * Aggiorna la barra del tempo.
 * @param bar       La barra.
 * @param board     Il puntatore alla board.
*/
void calculate_time_bar(Bar *bar, Board *board)
{
    calculate_bar(bar, board->max_time, board->time_left);
}

/**
 * Formatta i numeri del punteggio.
 * @param number    Il numero.
 * @param empty     Il carattere vuoto.
 * @param fill      Il carattere pieno.
*/
char **format_number(int number, char empty, char fill)
{
    if (number < 0 || number > 9)
    {
        return NULL;
    }

    char **result = MALLOC(char *, SCORE_HEIGHT);
    CRASH_IF_NULL(result);

    size_t height = 0, width = 0;

    result[height] = CALLOC_TERM(char, SCORE_WIDTH);
    CRASH_IF_NULL(result[height]);

    for (size_t i = 0; i < SCORE_LENGTH; i++)
    {
        char c = SCORES[number][i];

        if (c == ' ')
        {
            ++height;
            width = 0;

            result[height] = CALLOC_TERM(char, SCORE_WIDTH);
            CRASH_IF_NULL(result[height]);
        }
        else
        {
            result[height][width] = c == EMPTY_CHAR ? empty : c == FILL_CHAR ? fill : c;
            ++width;
        }
    }

    return result;
}
