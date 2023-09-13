#include "../../headers/graphics.h"

void send_menu(int may, int max, LOWCOST_INFO curr_indx) {
    erase();
    start_color();
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
    center_string("/.-./.--.|.--.\\          /.--.|.--.\\.-.|\n", max);
    attroff(COLOR_PAIR(1) | A_BOLD);
    // x0, y19
    switch(curr_indx) {
      case 0: {
        SELECTED_FIRST
      }
      break;
      case 1: {
        SELECTED_SECOND
      }
      break;
      case 2: {
        SELECTED_THIRD
      }
      break;
    }
    refresh();
}

LOWCOST_INFO menu_listener(int may, int max) {
    LOWCOST_INFO indx = 0;
    while(1) {
        int c = wgetch(stdscr);
        switch(c) {
          case KEY_UP: {
              indx -= (indx - 1) == -1 ? 0 : 1;
              send_menu(may, max, indx);
          }
          break;
          case KEY_DOWN: {
              indx += (indx + 1) == 3 ? 0 : 1;
              send_menu(may, max, indx);
          }
          break;
          case '\r':
          case '\n': {
              return indx;
          }
          break;
        }
    }
}

LOWCOST_INFO display(int max, int may) {
    send_menu(may, max, 0);
    return menu_listener(may, max);
}

void center_string(char str[], int max) {
    int sl = strlen(str);
    int total_size = (int)((max - sl) / 2);
    for (int i = 0; i < total_size; i++) {
      addch(' ');
    }
    addstr(str);
}




/*
  GAME LOGICS
*/

void display_board(Board *board) {
  int may = (*board).MAX_Y;
  int max = (*board).MAX_X;
  init_pair(HIDEOUTS_C, COLOR_BLACK, COLOR_GREEN);
  init_pair(RIVER_C, COLOR_WHITE, COLOR_BLUE);
  init_pair(GRASS_SIDEWALK_C, COLOR_BLACK, COLOR_GREEN);
  init_pair(ROAD_C, COLOR_WHITE, COLOR_BLACK);
  init_pair(SIDEWALK_C, COLOR_BLACK, COLOR_YELLOW);
  for (int i = 0; i < SIDEWALK; i++) {
    int x = -1;
    if (i < HIDEOUTS) x = 1;
    else if (i < RIVER) x = 2;
    else if (i < GRASS_SIDEWALK) x = 3;
    else if (i < ROAD) x = 4;
    else if (i < SIDEWALK) x = 5;
    attron(COLOR_PAIR(x));
    for (int j = 0, s = i*2; j < max; j++) {
      mvaddch(s, j, ' ');
      mvaddch(s+1, j, ' '); 
    }
    attroff(COLOR_PAIR(x));
  }
}































