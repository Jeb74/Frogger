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
    //raw();
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

/**
 * Stampa una stringa al centro del terminale.
 * @param str La stringa da stampare.
 * @param max La larghezza della finestra
 */
void wcenter_string(char str[], int max, WINDOW *w, int y)
{
    int sl = strlen(str);
    int total_size = (int)((max - sl) / 2);
    mvwaddstr(w, y, total_size, str);
}

/*
  GAME LOGICS
*/

void display_board(WINDOW *w, Board *board)
{
    int starting_y = board->top_y;
    int terminating_y = board->low_y;
    int width = w->_maxx;

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

        wattron(w, COLOR_PAIR(x));
        for (int j = 0, s = starting_y + k; j < width; j++)
        {
            mvwaddch(w, s, j, ' ');
            mvwaddch(w, s + 1, j, ' ');
        }
        wattroff(w, COLOR_PAIR(x));
        k += 2;
    }

    Position *pos = &(board->fp);
    //printf("%i - %i\n", terminating_y, pos->y);
    if (pos->x < 0) pos->x = 0;
    else if ((pos->x + 1) > width-1) pos->x = width - 2;
    if (pos->y < board->top_y + 2) pos->y = board->top_y + 2;
    else if (pos->y >= board->low_y) pos->y = board->low_y - 1;
    wattron(w, COLOR_PAIR(_COLOR_FROG));
    mvwaddch(w, pos->y, pos->x, 'o'),mvwaddch(w, pos->y, pos->x + 1, 'o');
    mvwaddch(w, pos->y + 1, pos->x, '('),mvwaddch(w, pos->y + 1, pos->x + 1, ')');
    wattroff(w,COLOR_PAIR(_COLOR_FROG));
    wrefresh(w);
}

void update_bar(WINDOW *w, char *title, int pc) {
    mvwaddstr(w, 0, 1, title);
    int y = (int)w->_maxy / 2 + 1; 
    int width = w->_maxx;
    int pair = pc > 75 ? _COLOR_HP_FULL : pc > 25 ? _COLOR_HP_PARTIAL
                                                : _COLOR_HP_EMPTY;

    for (int i = 1, once = true; i < width; i++) {
        if (once && i > (int)((double)width / 100 * pc)) {
            pair = _COLOR_RESET;
            once = false;
        }
        wattron(w, COLOR_PAIR(pair));
        mvwaddch(w, y, i, ' ');
        mvwaddch(w, y + 1, i, ' ');
        wattroff(w, COLOR_PAIR(pair));
    }
    wrefresh(w);
}

void update_score_bar_empty(WINDOW* w, int y, int *start, int leftovers) {
    wattron(w, COLOR_PAIR(_COLOR_RESET));
    for (int i = 0;  i < leftovers; i++) {
        for (int j = 0; j < SCORE_HEIGHT; j++) {
            mvwaddch(w, y+j, start+i, ' ');
        }
    }

    *start += leftovers;
    wattroff(w, COLOR_PAIR(_COLOR_RESET));
}

void update_score_bar(WINDOW *w, char *title, int value) {
    char *numb = num_to_string(value, 3);

    char **fdigit = format_number(numb[0] - '0', " ", "\xE2\x96\xAE");
    char **sdigit = format_number(numb[1] - '0', " ", "\xE2\x96\xAE");
    char **tdigit = format_number(numb[2] - '0', " ", "\xE2\x96\xAE");

    free(numb);
    int occ_len = SCORE_WIDTH*SCORE_WIDTH + 2;
    mvwaddstr(w, 0, 1, title);
    int leftovers = (int)((w->_maxx - occ_len) / 2);
    int start = 1;
    update_score_bar_empty(w, 1, &start, leftovers);
    wattron(w, COLOR_PAIR(_COLOR_SCORE));
    char *fmtd;
    for (int i = 0; i < SCORE_HEIGHT; i++) {
        fmtd = build_string("%s %s %s", fdigit[i], sdigit[i], tdigit[i]);
        mvwaddstr(w, i+1, start, fmtd);
        free(fmtd);
    }
    wattroff(w, COLOR_PAIR(_COLOR_SCORE));
    start+=occ_len;
    update_score_bar_empty(w, 1, &start, leftovers);
    FREE_ALL(fdigit, SCORE_HEIGHT);
    FREE_ALL(sdigit, SCORE_HEIGHT);
    FREE_ALL(tdigit, SCORE_HEIGHT);
    wrefresh(w);
}

void update_bars(Board *board, WINDOW *time, WINDOW *score, WINDOW *life) {
    Bar sc = {.value=board->points};
    Bar lf = create_life_bar(board);
    Bar tm = create_time_bar(board);
    char *str, *format;

    format = format_clock_numeric(board);
    str = build_string("Time Left: %s", format);
    update_bar(time, str, tm.value);
    free(format);
    free(str);

    format = format_lives_numeric(board);
    str = build_string("Remaining Health: %s", format);
    update_bar(life, str, lf.value);
    free(format);
    free(str);

    update_score_bar(score, "Score", sc.value);
}

WINDOW **create_windows(Screen screen) {
    int max = screen.x;
    int may = screen.y;
    int screen_mid = ((int)screen.y / 2);
    int top_y = screen_mid - (screen.y % 2 == 0) ? 8 : 9;
    int low_y = screen_mid + ((screen.y % 2 == 0) ? 16 : 15);
    WINDOW **ws = CALLOC(WINDOW*, WINDOWS);
    int tend = calwidth(max, INFO_BARS, TBAR);
    int send = calwidth(max, INFO_BARS, SBAR);
    int lend = calwidth(max, INFO_BARS, LBAR);
    ws[TBAR] = newwin(top_y-1, tend, 0, 0);
    ws[SBAR] = newwin(top_y-1, send, 0, tend);
    ws[LBAR] = newwin(top_y-1, lend, 0, tend + send);
    ws[GBOARD] = newwin(low_y - top_y + 1, max, top_y, 0);
    box(ws[TBAR], 0, 0); 
    box(ws[SBAR], 0, 0);
    box(ws[LBAR], 0, 0);
    return ws;
}

void show_pause_menu(WINDOW *w) {
    int width = w->_maxx;
    int height = w->_maxy+1;
    wattron(w, COLOR_PAIR(_COLOR_RESET));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            mvwaddch(w, i, j, ' ');
            if (j+1 == width && i == height / 2 - 1) {
                wattroff(w, COLOR_PAIR(_COLOR_RESET));
                wattron(w, COLOR_PAIR(_COLOR_MENU) | A_BOLD);
                wcenter_string("Resume", width, w, i);
                wattroff(w, COLOR_PAIR(_COLOR_MENU) | A_BOLD);
                wattron(w, COLOR_PAIR(_COLOR_RESET));
                break;
            }
        }
    }
    wattroff(w, COLOR_PAIR(_COLOR_RESET));
    wrefresh(w);
    int a;
    do {a = wgetch(w);}
    while(a != '\r' && a != '\n' && a != KEY_ESC);
}

void update_graphics(Board *board, EntityQueue *eq, WINDOW **ws) {
    update_bars(board, ws[TBAR], ws[SBAR], ws[LBAR]);
    display_board(ws[GBOARD], board);
}
