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
 * @param curr_indx L'indice corrente del menu'.
 */
void send_menu(int may, int max, LOWCOST_INFO curr_indx)
{
    erase();

    init_pair(1, COLOR_GREEN, COLOR_BLACK);  // FROG
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // SELECTED
    init_pair(3, COLOR_RED, COLOR_BLACK);    // QUIT

    attron(COLOR_PAIR(1) | A_BOLD);
    center_string("____  __.---\"\"---.__  ____\n", max);
    center_string("/####\\/              \\/####\\\n", max);
    center_string("(   /\\ )              ( /\\   )\n", max);
    center_string("\\____/                \\____/\n", max);
    center_string("__/                          \\__\n", max);
    center_string(".-\"    .                      .    \"-.\n", max);
    center_string("|  |   \\.._                _../   |  |\n", max);
    center_string("\\  \\    \\.\"-.__________.-\"./    /  /\n", max);
    center_string("\\  \\    \"--.________.--\"    /  /\n", max);
    center_string("___\\  \\_                    _/  /___\n", max);
    center_string("./    )))))                  (((((    \\.\n", max);
    center_string("\\                                      /\n", max);
    center_string("\\           \\_          _/           /\n", max);
    center_string("\\    \\____/\"\"-.____.-\"\"\\____/    /\n", max);
    center_string("\\    \\                  /    /\n", max);
    center_string("\\.  .|                |.  ./\n", max);
    center_string(".\" / |  \\              /  | \\  \".\n", max);
    center_string(".\"  /   |   \\            /   |   \\   \".\n", max);
    center_string("/.-./.--.|.--.\\          /.--.|.--.\\.-.|\n\n\n", max);

    attroff(COLOR_PAIR(1) | A_BOLD);

    // x0, y19
    switch (curr_indx)
    {
        case 0:
        {
            SELECTED_FIRST
        }
        break;
        case 1:
        {
            SELECTED_SECOND
        }
        break;
        case 2:
        {
            SELECTED_THIRD
        }
        break;
    }

    refresh();
}

/**
 * Gestisce l'input del menu' da parte dell'utente.
 * @param may L'altezza della finestra.
 * @param max La larghezza della finestra.
 */
LOWCOST_INFO menu_listener(int may, int max)
{
    LOWCOST_INFO indx = 0;

    while (true)
    {
        switch (getch())
        {
            case KEY_UP:
            {
                indx -= (indx - 1) == -1 ? 0 : 1;
                send_menu(may, max, indx);
            }
            break;
            case KEY_DOWN:
            {
                indx += (indx + 1) == 3 ? 0 : 1;
                send_menu(may, max, indx);
            }
            break;
            case '\r':
            case '\n':
            {
                return indx;
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

    for (int i = 0, k = 0; i < SIDEWALK && starting_y + k < terminating_y; i++)
    {
        int x = -1;

        if (i < HIDEOUTS)
            x = HIDEOUTS_C;
        else if (i < RIVER)
            x = RIVER_C;
        else if (i < GRASS_SIDEWALK)
            x = GRASS_SIDEWALK_C;
        else if (i < ROAD)
            x = ROAD_C;
        else if (i < SIDEWALK)
            x = SIDEWALK_C;

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
    //printf("%i - %i\n", terminating_y, pos->y);
    if (pos->x < 0) pos->x = 0;
    else if ((pos->x + 1) > width-1) pos->x = width - 2;
    if (pos->y < board->top_y + 2) pos->y = board->top_y + 2;
    else if (pos->y >= board->low_y) pos->y = board->low_y - 1;
    attron(COLOR_PAIR(FROG));
    mvaddch(pos->y, pos->x, 'o'),mvaddch(pos->y, pos->x + 1, 'o');
    mvaddch(pos->y + 1, pos->x, '('),mvaddch(pos->y + 1, pos->x + 1, ')');
    attroff(COLOR_PAIR(FROG));
}

void update_bar(int y, int start, int end, int pc, char title[])
{
    end = end - start;
    mvaddstr(y - 1, start, title);
    int pair = pc > 75 ? HP_FULL_SLOT : pc > 25 ? HP_PARTIAL_SLOT
                                                : HP_EMPTY_SLOT;

    for (int i = 0, once = true; i < end; i++)
    {
        if (i > (float)end / 100 * pc && once)
        {
            pair = RESET_COLOR;
            once = false;
        }
        attron(COLOR_PAIR(pair));
        mvaddch(y, start + i, ' ');
        mvaddch(y + 1, start + i, ' ');
        attroff(COLOR_PAIR(pair));
    }
}

void update_score_bar_empty(int y, int *start, int leftovers) {
    attron(COLOR_PAIR(RESET_COLOR));
    for (int i = 0;  i < leftovers; i++) {
        for (int j = 0; j < SCORE_HEIGHT; j++) {
            mvaddch(y+j, start+i, ' ');
        }
    }
    *start += leftovers;
    attroff(COLOR_PAIR(RESET_COLOR));
}

void update_score_bar(int y, int start, int end, int value, char title[]) {
    char *numb = num_to_string(value, 3);
    char **fdigit = format_number(numb[0] - '0', " ", "\xE2\x96\xAE");
    char **sdigit = format_number(numb[1] - '0', " ", "\xE2\x96\xAE");
    char **tdigit = format_number(numb[2] - '0', " ", "\xE2\x96\xAE");
    free(numb);
    int occ_len = SCORE_WIDTH*SCORE_WIDTH + 2;
    mvaddstr(y-1, start, title);
    int leftovers = (int)(((end - start) - occ_len) / 2);
    update_score_bar_empty(y, &start, leftovers);
    attron(COLOR_PAIR(SCORE_C));
    char *fmtd;
    for (int i = 0; i < SCORE_HEIGHT; i++) {
        fmtd = build_string("%s %s %s", fdigit[i], sdigit[i], tdigit[i]);
        mvaddstr(y+i, start, fmtd);
        free(fmtd);
    }
    attroff(COLOR_PAIR(SCORE_C));
    start+=occ_len;
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
    update_bar(location_pos, lcs_3 + 1, lce_3 - 1, (int)lf.value, str);
    free(format);
    free(str);

    format = format_clock_numeric(&board);
    str = build_string("Time Left: %s", format);
    update_bar(location_pos, lcs_1 + 1, lcs_2 - 1, (int)tm.value, str);
    free(format);
    free(str);
    location_pos = location_pos >= 2 ? location_pos - 2 : location_pos;
    update_score_bar(location_pos, lcs_2 + 1, lcs_3 - 1, (int)(sc.value), "Score:");
}

void update_graphics(Board *board)
{
    Bar sc = {.value=board->points};
    update_bars(*board, create_life_bar(board), create_time_bar(board), sc);
    display_board(board);
    refresh();
}
