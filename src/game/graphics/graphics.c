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

    for (int i = 0; i < SIDEWALK; i++)
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
        for (int j = 0, s = starting_y + i * 2; j < width; j++)
        {
            mvaddch(s, j, ' ');
            mvaddch(s + 1, j, ' ');
        }
        attroff(COLOR_PAIR(x));
    }
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

void update_bars(Board board, Bar lf, Bar tm)
{
    int location_pos = board.top_y / 2 - 1;
    int lcs_1 = 0;
    int lcs_2 = lcs_1 + calwidth(board.screen_x, 3, 0);
    int lcs_3 = lcs_2 + calwidth(board.screen_x, 3, 1);
    int lce_3 = lcs_3 + calwidth(board.screen_x, 3, 2);
    char str[64];
    char *format = format_lives_numeric(&board);
    sprintf(str, "Remaining Health: %s", format);
    update_bar(location_pos, lcs_3 + 1, lce_3 - 1, (int)lf.percent_full, str);
    free(format);
    format = format_clock_numeric(&board);
    sprintf(str, "Time Left: %s", format);
    free(format);
    update_bar(location_pos, lcs_1 + 1, lcs_2 - 1, (int)tm.percent_full, str);
}

void update_graphics(Board *board, Bar lf, Bar tm)
{
    update_bars(*board, lf, tm);
    display_board(board);
    refresh();
}
