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

LOWCOST_INFO getAction(int argc, char *argv[])
{
    LOWCOST_INFO rslt = 0;

    if (argc > 3)
    {
        perror("Too many arguments: ./frogger <OPTIONAL:execution_type> <OPTIONAL:quit_after_execution>\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < argc; i++)
    {
        if (strcmp("-p", argv[i]) == 0)
            rslt += 1;
        else if (strcmp("-t", argv[i]) == 0)
            rslt += 0;
        else if (strcmp("-q", argv[i]) == 0)
            rslt += 3;
    }

    if (rslt > 4)
    {
        perror("Wrong arguments: ./frogger <OPTIONAL:execution_type> <OPTIONAL:quit_after_execution>\n");
        exit(EXIT_FAILURE);
    }

    return rslt;
}

/**
 * Calcola la larghezza di un pannello.
 * @param width     La larghezza totale.
 * @param panels    Il numero di pannelli.
 * @param indx      L'indice del pannello.
 */
unsigned int calwidth(unsigned int width, unsigned int panels, LOWCOST_INFO indx)
{
    int defsize = width / panels;
    int result = defsize;

    if (width != panels * defsize)
    {
        int leftovers = width % panels;
        bool is_center = indx == panels / 2 + 1;

        if (panels % 2 == 0)
        {
            if (is_center || indx == panels / 2)
            {
                return defsize + leftovers / 2;
            }
        }
        else if (is_center)
        {
            return defsize + leftovers;
        }
    }

    return defsize;
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

    return ((double)dividend / divisor) * 100;
}

/**
 * Crea una barra.
 * @param board     Il puntatore alla board.
 * @param max       Il massimo.
 * @param current   Il valore corrente.
 * @return          La barra.
 */
Bar create_bar(Board *board, int max, int current)
{
    Bar bar;
    calculate_bar(&bar, max, current);

    return bar;
}

/**
 * Crea una barra della vita.
 * @param board Il puntatore alla board.
 * @param size  La dimensione della barra.
 * @return      La barra.
 */
Bar create_life_bar(Board *board)
{
    return create_bar(board, board->lifes_on_start, board->lifes_left);
}

/**
 * Crea una barra del tempo.
 * @param board Il puntatore alla board.
 * @param size  La dimensione della barra.
 * @return      La barra.
 */
Bar create_time_bar(Board *board)
{
    return create_bar(board, board->max_time, board->time_left);
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

    size_t height = 0, width = 0;

    char **result = MALLOC(char *, SCORE_HEIGHT);
    CRASH_IF_NULL(result);

    result[height] = CALLOC_TERM(char, SCORE_WIDTH);
    CRASH_IF_NULL(result[height]);

    for (size_t i = 0; i < SCORE_LENGTH; i++)
    {
        char c = SCORES[number][i];

        if (c == NULL_CHAR)
        {
            ++height;
            width = 0;

            result[height] = CALLOC_TERM(char, SCORE_WIDTH);
            CRASH_IF_NULL(result[height]);
        }
        else
        {
            result[height][width] = c == EMPTY_CHAR ? empty : c == FILL_CHAR ? fill
                                                                             : c;
            ++width;
        }
    }

    return result;
}

/**
 * Converte un numero in una stringa.
 * @param num   Il numero.
 * @param size  La dimensione della stringa.
 * @param fill  Se riempire la stringa con zeri.
*/
char *num_to_string(int num, unsigned int size, bool fill)
{
    int dim = 0;
    size = size > 0 ? size : 11;

    char *numb = CALLOC(char, size);
    numb[0] = '0';

    while (num != 0 || fill)
    {
        /* Sposta indietro la cifra. */
        for (int i = dim; dim != 0 && i > -1; i--)
        {
            numb[i] = numb[i - 1];
        }

        dim++;
        numb[0] = (num % 10) + '0';

        if (num > 0)
        {
            num /= 10;
        }

        if (strlen(numb) == size && fill)
        {
            fill = !fill;
        }
    }

    size = strlen(numb);
    numb = REALLOC(char, numb, size + TERM);
    numb[size] = '\0';

    return numb;
}

/**
 * Sfrutta dei placeholder e dei varargs per creare una stringa.
 * @param format    Il formato della stringa (con placeholders).
 * @param ...       I varargs.
*/
char *build_string(const char *__restrict_arr format, ...)
{
    int slen = strlen(format);
    int count = 0;
    char **elements = CALLOC(char *, 30);
    
    va_list args;
    va_start(args, format);

    for (int i = 0; i < slen; i++)
    {
        if (format[i] == '%' && format[i + 1] == 's')
        {
            elements[count] = va_arg(args, char *);
            slen += strlen(elements[count]);
            count++;
        }
    }

    char *output = CALLOC(char, slen + TERM);

    for (int i = 0, j = 0, q = 0; i < slen; i++, q++)
    {
        if (format[q] == '%' && format[q + 1] == 's')
        {
            q += 1;

            for (int k = 0; k < strlen(elements[j]); k++, i++)
            {
                output[i] = elements[j][k];
            }

            i--, j++;
        }
        else
            output[i] = format[q];
    }

    free(elements);

    int ol = strlen(output);
    output = REALLOC(char, output, ol + TERM);
    output[ol] = 0;

    return output;
}
