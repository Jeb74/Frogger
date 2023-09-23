#include "../../headers/graphics.h"

/**
 * Inizializza la grafica.
 * @param board La tabella di gioco.
 */
void init_graphics(Screen *scrn)
{
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    getmaxyx(stdscr, scrn->y, scrn->x);

    curs_set(false);
    keypad(stdscr, true);
    start_color();
}

/**
 * Menu' di selezione della modalita' da utilizzare per il gioco.
 * @param max La larghezza della finestra.
 * @param may L'altezza della finestra.
 * @param current_choice L'indice corrente del menu'.
 */
void send_menu(int may, int max, MenuChoice current_choice)
{
    erase();

    init_pair(_COLOR_MENU_GREEN, COLOR_GREEN, COLOR_BLACK);   // FROG
    init_pair(_COLOR_MENU_YELLOW, COLOR_YELLOW, COLOR_BLACK); // SELECTED
    init_pair(_COLOR_MENU_RED, COLOR_RED, COLOR_BLACK);       // QUIT

    attron(COLOR_PAIR(_COLOR_MENU_GREEN) | A_BOLD);
    for (size_t i = 0; i < _FROG_ART_LENGTH; i++)
    {
        center_string(_FROG_ART[i], max);
    }
    attroff(COLOR_PAIR(_COLOR_MENU_GREEN) | A_BOLD);

    // x0, y19

    if (current_choice == QUIT)
    {
        print_logo(_COLOR_MENU_RED, max);
    }
    else
    {
        print_logo(_COLOR_MENU_YELLOW, max);
    }

    print_choices(current_choice, max);
    refresh();
}

/**
 * Stampa le scelte del menu'.
 * @param current_choice L'indice corrente del menu'.
 * @param max La larghezza della finestra.
 */
void print_choices(MenuChoice current_choice, int max)
{
    for (size_t i = 0; i < _MENU_CHOICES_LENGTH; i++)
    {
        if (current_choice == i)
        {
            sprint_colored(_MENU_CHOICES_SELECTED[i], _COLOR_MENU_YELLOW, max);
            continue;
        }

        sprint_colored(_MENU_CHOICES[i], i == QUIT ? _COLOR_MENU_RED : _COLOR_MENU_GREEN, max);
    }
}

/**
 * Stampa una stringa al centro del terminale.
 * @param string La stringa da stampare.
 * @param color Il colore da utilizzare.
 * @param max La larghezza della finestra
 */
void sprint_colored(char *string, int color, int max)
{
    attron(COLOR_PAIR(color));
    center_string(string, max);
    attroff(COLOR_PAIR(color));
}

/**
 * Stampa il logo del gioco.
 * @param color Il colore da utilizzare.
 * @param max La larghezza della finestra
 */
void print_logo(int color, int max)
{
    attron(COLOR_PAIR(color) | A_BOLD);
    for (size_t i = 0; i < _FROGGER_LOGO_LENGTH; i++)
    {
        center_string(_FROGGER_LOGO[i], max);
    }
    attroff(COLOR_PAIR(color) | A_BOLD);
}

/**
 * Gestisce l'input del menu' da parte dell'utente.
 * @param may L'altezza della finestra.
 * @param max La larghezza della finestra.
 */
MenuChoice menu_listener(int may, int max)
{
    MenuChoice choice = START_THREADS;

    while (true)
    {
        switch (getch())
        {
        case KEY_UP:
        {
            if (choice != START_THREADS)
            {
                send_menu(may, max, --choice);
            }
        }
        break;
        case KEY_DOWN:
        {
            if (choice != QUIT)
            {
                send_menu(may, max, ++choice);
            }
        }
        break;
        case '\r':
        case '\n':
        {
            return choice;
        }
        break;
        }
    }
}

/**
 * Menu' di selezione della modalita' da utilizzare per il gioco.
 * @param max La larghezza della finestra.
 * @param may L'altezza della finestra.
 */
LOWCOST_INFO initial_menu(int max, int may)
{
    send_menu(may, max, 0);
    return menu_listener(may, max);
}

/**
 * Stampa una stringa al centro del terminale.
 * @param str La stringa da stampare.
 * @param max La larghezza della finestra
 */
void center_string(char str[], int max)
{
    int sl = strlen(str);
    int total_size = (int)((max - sl) / 2);

    for (int i = 0; i < total_size; i++)
    {
        addch(' ');
    }

    addstr(str);
}

/*
  GAME LOGICS
*/

void display_board(Board *board)
{
    int starting_y = board->top_y;
    int terminating_y = board->low_y;
    int width = board->screen_x;

    for (int i = 0, k = 0; i < _Y_SIDEWALK && starting_y + k < terminating_y; i++)
    {
        int x = -1;

        if (i < _Y_HIDEOUT)
            x = _COLOR_HIDEOUT;
        else if (i < _Y_RIVER)
            x = _COLOR_RIVER;
        else if (i < _Y_LAWN)
            x = _COLOR_LAWN;
        else if (i < _Y_ROAD)
            x = _COLOR_ROAD;
        else if (i < _Y_SIDEWALK)
            x = _COLOR_SIDEWALK;

        attron(COLOR_PAIR(x));
        for (int j = 0, s = starting_y + k; j < width; j++)
        {
            mvaddch(s, j, ' ');
            mvaddch(s + 1, j, ' ');
        }
        attroff(COLOR_PAIR(x));
        k += 2;
    }

    Position *pos = &(board->fp);

    // TODO REFACTOR
    if (pos->x < 0)
        pos->x = 0;
    else if ((pos->x + 1) > width - 1)
        pos->x = width - 2;
    
    if (pos->y < board->top_y + 2)
        pos->y = board->top_y + 2;
    else if (pos->y >= board->low_y)
        pos->y = board->low_y - 1;

    attron(COLOR_PAIR(_COLOR_FROG));
    mvaddch(pos->y, pos->x, 'o'), mvaddch(pos->y, pos->x + 1, 'o');
    mvaddch(pos->y + 1, pos->x, '('), mvaddch(pos->y + 1, pos->x + 1, ')');
    attroff(COLOR_PAIR(_COLOR_FROG));
}

void update_bar(int y, int start, int end, int pc, char title[])
{
    end = end - start;
    mvaddstr(y - 1, start, title);
    int pair = pc > 75 ? _COLOR_HP_FULL : pc > 25 ? _COLOR_HP_PARTIAL
                                                  : _COLOR_HP_EMPTY;

    for (int i = 0, once = true; i < end; i++)
    {
        if (i > (float)end / 100 * pc && once)
        {
            pair = _COLOR_RESET;
            once = false;
        }
        attron(COLOR_PAIR(pair));
        mvaddch(y, start + i, ' ');
        mvaddch(y + 1, start + i, ' ');
        attroff(COLOR_PAIR(pair));
    }
}

void update_score_bar_empty(int y, int *start, int leftovers)
{
    attron(COLOR_PAIR(_COLOR_RESET));

    for (int i = 0; i < leftovers; i++)
    {
        for (int j = 0; j < SCORE_HEIGHT; j++)
        {
            mvaddch(y + j, start + i, ' ');
        }
    }

    *start += leftovers;
    attroff(COLOR_PAIR(_COLOR_RESET));
}

void update_score_bar(int y, int start, int end, int value, char title[])
{
    char *numb = num_to_string(value, 3);

    char **fdigit = format_number(numb[0] - '0', " ", "\xE2\x96\xAE");
    char **sdigit = format_number(numb[1] - '0', " ", "\xE2\x96\xAE");
    char **tdigit = format_number(numb[2] - '0', " ", "\xE2\x96\xAE");

    free(numb);

    mvaddstr(y - 1, start, title);

    int occ_len = SCORE_WIDTH * SCORE_WIDTH + 2;
    int leftovers = (int)(((end - start) - occ_len) / 2);
    update_score_bar_empty(y, &start, leftovers);

    char *temp;
    attron(COLOR_PAIR(_COLOR_SCORE));
    for (int i = 0; i < SCORE_HEIGHT; i++)
    {
        temp = build_string("%s %s %s", fdigit[i], sdigit[i], tdigit[i]);
        mvaddstr(y + i, start, temp);
        free(temp);
    }
    attroff(COLOR_PAIR(_COLOR_SCORE));

    start += occ_len;
    update_score_bar_empty(y, &start, leftovers);

    FREE_ALL(fdigit, SCORE_HEIGHT);
    FREE_ALL(sdigit, SCORE_HEIGHT);
    FREE_ALL(tdigit, SCORE_HEIGHT);
}

void update_bars(Board board, Bar lf, Bar tm, Bar sc)
{
    int location_pos = board.top_y / 2 - 1;
    int lcs_1 = 0;
    int lcs_2 = lcs_1 + calwidth(board.screen_x, 3, 0);
    int lcs_3 = lcs_2 + calwidth(board.screen_x, 3, 1);
    int lce_3 = lcs_3 + calwidth(board.screen_x, 3, 2);

    char *str;
    char *format = format_lives_numeric(&board);

    str = build_string("Remaining Health: %s", format);
    update_bar(location_pos, lcs_3 + 1, lce_3 - 1, lf.value, str);
    free(format);
    free(str);

    format = format_clock_numeric(&board);
    str = build_string("Time Left: %s", format);
    update_bar(location_pos, lcs_1 + 1, lcs_2 - 1, tm.value, str);
    free(format);
    free(str);

    location_pos = location_pos >= 2 ? location_pos - 2 : location_pos;
    update_score_bar(location_pos, lcs_2 + 1, lcs_3 - 1, sc.value, "Score:");
}

void update_graphics(Board *board, EntityQueue *eq)
{
    static Bar score;
    static Bar life;
    static Bar timer;

    calculate_score_bar(&score, board);
    calculate_life_bar(&life, board);
    calculate_time_bar(&timer, board);

    update_bars(*board, life, timer, score);

    display_board(board);
    refresh();
}
