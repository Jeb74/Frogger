#include "../../headers/graphics.h"


int display(bool *started) {
    if (*started) return -1;
    int max;
    int may;
    init_game_menu_scrn(&may, &max);
    send_menu(may, max, 0);
    return menu_listener(may, max);
}


void init_game_menu_scrn(int *y, int *x) {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
    curs_set(false);
    getmaxyx(stdscr, *y, *x);
}

void center_string(char str[], int max) {
    int sl = strlen(str);
    int total_size = (int)((max - sl) / 2);
    for (int i = 0; i < total_size; i++) {
      addch(' ');
    }
    addstr(str);
}

void send_menu(int may, int max, int curr_indx) {
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

int menu_listener(int may, int max) {
    unsigned indx = 0;
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