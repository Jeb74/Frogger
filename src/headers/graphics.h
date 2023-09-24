#ifndef FROGGER_GRAPHICS_C
#define FROGGER_GRAPHICS_C

#include "structures.h"

#define KEY_ESC 27

#define KEY_ESC 27

/* Color codes start */

#define _COLOR_MENU_GREEN 1
#define _COLOR_MENU_YELLOW 2
#define _COLOR_MENU_RED 3

#define _COLOR_RESET 1
#define _COLOR_HIDEOUT 2
#define _COLOR_RIVER 3
#define _COLOR_LAWN 4
#define _COLOR_ROAD 5
#define _COLOR_SIDEWALK 6
#define _COLOR_HP_FULL 7
#define _COLOR_HP_PARTIAL 8
#define _COLOR_HP_EMPTY 9
#define _COLOR_SCORE 10
#define _COLOR_FROG 11
#define _COLOR_MENU 12
#define _COLOR_WON _COLOR_FROG
#define _COLOR_LOST 13
#define _COLOR_PROJECTILE _COLOR_ROAD
#define _COLOR_ENEMY_SNAKE _COLOR_LOST
#define _COLOR_ENEMY_BIRD _COLOR_LOST
#define _COLOR_ENEMY_FROG _COLOR_LOST
#define _COLOR_CAR _COLOR_LOST
#define _COLOR_TRUCK _COLOR_MENU
#define _COLOR_LOG _COLOR_SIDEWALK

static char _I_PROJECTILE[2][8] = {
    "*       ",
    "        "
}
static char _I_SNAKE[2][8] = {
    " /\\ /\\ /",
    "v \\/ \\/ "
}
static char _I_BIRD[2][8] = {
    "()      ",
    "ov      "
}
static char _I_ENEMY_FROG[2][8] = {
    "()      ",
    "oo       "
}
static char _I_LOG[2][8] = {
    "        ",
    "        "
}
static char _I_TRUCK[2][8] = {
    "[######]",
    "[######]"
}
#define _I_CAR _I_TRUCK


/* Color codes end */

#define _Y_HIDEOUT 2
#define _Y_RIVER 7
#define _Y_LAWN 8
#define _Y_ROAD 13
#define _Y_SIDEWALK 14

#define INIT_COLORS                                              \
    {                                                            \
        init_pair(_COLOR_RESET, COLOR_BLACK, COLOR_BLACK);       \
                                                                 \
        init_pair(_COLOR_HIDEOUT, COLOR_BLACK, COLOR_GREEN);     \
        init_pair(_COLOR_RIVER, COLOR_BLACK, COLOR_BLUE);        \
        init_pair(_COLOR_LAWN, COLOR_BLACK, COLOR_GREEN);        \
        init_pair(_COLOR_ROAD, COLOR_WHITE, COLOR_BLACK);        \
        init_pair(_COLOR_SIDEWALK, COLOR_BLACK, COLOR_YELLOW);   \
                                                                 \
        init_pair(_COLOR_HP_FULL, COLOR_WHITE, COLOR_GREEN);     \
        init_pair(_COLOR_HP_PARTIAL, COLOR_WHITE, COLOR_YELLOW); \
        init_pair(_COLOR_HP_EMPTY, COLOR_WHITE, COLOR_RED);      \
        init_pair(_COLOR_SCORE, COLOR_YELLOW, COLOR_BLACK);      \
                                                                 \
        init_pair(_COLOR_FROG, COLOR_GREEN, COLOR_BLACK);        \
        init_pair(_COLOR_MENU, COLOR_YELLOW, COLOR_BLACK);       \
        init_pair(_COLOR_LOST, COLOR_RED, COLOR_BLACK);          \
        
    }

#define INFO_BARS 3
#define WINDOWS 4
#define TBAR 0
#define SBAR 1
#define LBAR 2
#define GBOARD 3

#define _FROG_ART_LENGTH 20
static char *_FROG_ART[_FROG_ART_LENGTH] =
    {
        "\n",
        "____  __.---\"\"---.__  ____\n",
        "/####\\/              \\/####\\\n",
        "(   /\\ )              ( /\\   )\n",
        "\\____/                \\____/\n",
        "__/                          \\__\n",
        ".-\"    .                      .    \"-.\n",
        "|  |   \\.._                _../   |  |\n",
        " \\  \\    \\.\"-.__________.-\"./    /  /\n",
        "\\  \\    \"--.________.--\"    /  /\n",
        "___\\  \\_                    _/  /___\n",
        "./    )))))                  (((((    \\.\n",
        "\\                                      /\n",
        "\\           \\_          _/           /\n",
        "\\    \\____/\"\"-.____.-\"\"\\____/    /\n",
        "\\    \\                  /    /\n",
        "\\.  .|                |.  ./\n",
        " .\" / |  \\              /  | \\  \".\n",
        ".\"  /   |   \\            /   |   \\   \".\n",
        "/.-./.--.|.--.\\          /.--.|.--.\\.-.|\"\n"
    };

#define _FROGGER_LOGO_LENGTH 6
static char *_FROGGER_LOGO[_FROGGER_LOGO_LENGTH] =
    {
        "    ______                                \n",
        "   / ____/________  ____ _____ ____  _____\n",
        "  / /_  / ___/ __ \\/ __ `/ __ `/ _ \\/ ___/\n",
        " / __/ / /  / /_/ / /_/ / /_/ /  __/ /    \n",
        "/_/   /_/   \\____/\\__, /\\__, /\\___/_/     \n",
        "                 /____//____/             \n\n"
    };

#define _MENU_CHOICES_LENGTH 3
static char *_MENU_CHOICES[_MENU_CHOICES_LENGTH] =
    {
        "Start threads version\n",
        "Start processes version\n",
        "Quit\n"
    };

static char *_MENU_CHOICES_SELECTED[_MENU_CHOICES_LENGTH] =
    {
        "> Start threads version <\n",
        "> Start processes version <\n",
        "> Quit <\n"
    };

typedef enum
{
    START_THREADS,
    START_PROCESSES,
    QUIT
} MenuChoice;

void init_graphics(Screen *scrn);
void center_string(char str[], int max);
WINDOW **create_windows(Screen screen);
void show_pause_menu(WINDOW *w);
void send_win_menu(WINDOW *w);
void send_lose_menu(WINDOW *w);
LOWCOST_INFO initial_menu(int max, int may);
void update_graphics(Board *board, EntityQueue *eq, WINDOW *ws[4]);

void print_choices(MenuChoice current_choice, int max);
void sprint_colored(char *string, int color, int max);
void print_logo(int color, int max);

#endif