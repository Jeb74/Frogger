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

int rand_pick(int c1, int c2)
{
    switch(rand()%2) {
        case 0: return c1;
        case 1: return c2;
    }
}

/**
 * Termina il gioco.
 * @param board Il puntatore alla board.
 */
void end_game(Board *board)
{
    endwin();
}

/**
 * Considera gli argomenti del programma.
 * @param argc  Il numero di argomenti.
 * @param argv  Gli argomenti.
*/
LOWCOST_INFO get_action(int argc, char *argv[])
{
    LOWCOST_INFO result = 0;

    if (argc > 3)
    {
        perror("Too many arguments: ./frogger <OPTIONAL:execution_type> <OPTIONAL:quit_after_execution>\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < argc; i++)
    {
        if (strcmp("-p", argv[i]) == 0)
            result += 1;
        else if (strcmp("-t", argv[i]) == 0)
            result += 0;
        else if (strcmp("-q", argv[i]) == 0)
            result += 3;
    }

    if (result > 4)
    {
        perror("Wrong arguments: ./frogger <OPTIONAL:execution_type> <OPTIONAL:quit_after_execution>\n");
        exit(EXIT_FAILURE);
    }

    return result;
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
Bar create_bar(int max, int current)
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
    return create_bar(board->lifes_on_start, board->lifes_left);
}

/**
 * Crea una barra del tempo.
 * @param board Il puntatore alla board.
 * @param size  La dimensione della barra.
 * @return      La barra.
 */
Bar create_time_bar(Board *board)
{
    return create_bar(board->max_time, board->time_left);
}

/**
 * Crea una barra del punteggio.
 * @param board Il puntatore alla board.
 * @return     La barra.
 */
Bar create_score_bar(Board *board)
{
    return (Bar) {.value = board->points};
}

/**
 * Aggiorna la barra.
 * @param bar       La barra.
 * @param max       Il massimo.
 * @param current   Il valore corrente.
 */
void calculate_bar(Bar *bar, int max, int current)
{
    bar->value = (int) calculate_percentage(current, max);
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
 * Aggiorna la barra del punteggio.
 * @param bar       La barra.
 * @param board     Il puntatore alla board.
 */
void calculate_score_bar(Bar *bar, Board *board)
{
    bar->value = board->points;
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
    char **elements = CALLOC_TERM(char *, count);

    va_list args;
    va_start(args, format);

    for (int i = 0, z = slen; i < z; i++)
    {
        if (format[i] == '%' && format[i + 1] == 's')
        {
            char *str = (char*) va_arg(args, char *);
            elements[count] = str;
            slen += strlen(str);
            
	        ++count;
            elements = REALLOC_TERM(char *, elements, count);
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
    output = REALLOC_TERM(char, output, ol);
    output[ol] = '\0';
    va_end(args);
    return output;
}

/**
 * Formatta i numeri del punteggio.
 * @param number    Il numero.
 * @param empty     Il carattere vuoto.
 * @param fill      Il carattere pieno.
 */
char **format_number(int number, char empty[], char fill[])
{
    if (number < 0 || number > 9)
    {
        return NULL;
    }

    size_t height = 0;
    int size_of_char = strlen(fill);
    int size_of_echar = strlen(empty);

    char **result = MALLOC(char *, SCORE_HEIGHT);
    CRASH_IF_NULL(result);

    char *translated[3];

    for (size_t i = 0; i < SCORE_LENGTH; i += 4)
    {
        translated[0] = SCORES[number][i]   == EMPTY_CHAR ? empty : fill;
        translated[1] = SCORES[number][i + 1] == EMPTY_CHAR ? empty : fill;
        translated[2] = SCORES[number][i + 2] == EMPTY_CHAR ? empty : fill;

        result[height] = build_string("%s%s%s", translated[0], translated[1], translated[2]);
        height++;
    }

    return result;
}

/**
 * Converte un numero in una stringa.
 * @param num   Il numero.
 * @param size  La dimensione della stringa.
 * @param fill  Se riempire la stringa con zeri.
 */
char *num_to_string(int num, int size)
{
    bool fill = size > 0;
    size = fill ? size : count_digits(num);

    int ts = 0;

    char *numb = CALLOC_TERM(char, size);
    numb[size] = '\0';
    numb[0] = '0';

    while (num != 0 || fill)
    {   
        /* Sposta indietro la cifra. */
        for (int i = ts; ts != 0 && i > 0; i--)
        {
            numb[i] = numb[i - 1];
        }

        numb[0] = (num % 10) + '0';

        if (num > 0)
        {
            num /= 10;
        }

        fill = fill ? ts != size - 1 : false;
        ts++;
    }

    size = strlen(numb);
    numb = REALLOC_TERM(char, numb, size);
    numb[size] = '\0';

    return numb;
}
