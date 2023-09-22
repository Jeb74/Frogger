#ifndef FROGGER_GRAPHICS_C
#define FROGGER_GRAPHICS_C

#include "structures.h"

#define SELECTED_FIRST                                  \
    attron(COLOR_PAIR(2) | A_BOLD);                     \
    center_string("> Start threads version <\n", max);  \
    attroff(COLOR_PAIR(2) | A_BOLD);                    \
    attron(COLOR_PAIR(1));                              \
    center_string("Start processes version\n", max);    \
    attroff(COLOR_PAIR(1));                             \
    attron(COLOR_PAIR(3));                              \
    center_string("Quit\n", max);                       \
    attroff(COLOR_PAIR(3));

#define SELECTED_SECOND                                     \
    attron(COLOR_PAIR(1));                                  \
    center_string("Start threads version\n", max);          \
    attroff(COLOR_PAIR(1));                                 \
    attron(COLOR_PAIR(2) | A_BOLD);                         \
    center_string("> Start processes version <\n", max);    \
    attroff(COLOR_PAIR(2) | A_BOLD);                        \
    attron(COLOR_PAIR(3));                                  \
    center_string("Quit\n", max);                           \
    attroff(COLOR_PAIR(3));

#define SELECTED_THIRD                               \
    attron(COLOR_PAIR(1));                           \
    center_string("Start threads version\n", max);   \
    attroff(COLOR_PAIR(1));                          \
    attron(COLOR_PAIR(1));                           \
    center_string("Start processes version\n", max); \
    attroff(COLOR_PAIR(1));                          \
    attron(COLOR_PAIR(2) | A_BOLD);                  \
    center_string("> Quit <\n", max);                \
    attroff(COLOR_PAIR(2) | A_BOLD);

#define HIDEOUTS 2
#define RIVER 7
#define GRASS_SIDEWALK 8
#define ROAD 13
#define SIDEWALK 14

#define RESET_COLOR 1

#define HIDEOUTS_C 2
#define RIVER_C 3
#define GRASS_SIDEWALK_C 4
#define ROAD_C 5
#define SIDEWALK_C 6

#define HP_FULL_SLOT 7
#define HP_PARTIAL_SLOT 8
#define HP_EMPTY_SLOT 9

#define SCORE_C 10

#define FROG 11

#define INIT_COLORS                                         \
{                                                           \
    init_pair(RESET_COLOR, COLOR_BLACK, COLOR_BLACK);       \
                                                            \
    init_pair(HIDEOUTS_C, COLOR_BLACK, COLOR_GREEN);        \
    init_pair(RIVER_C, COLOR_BLACK, COLOR_BLUE);            \
    init_pair(GRASS_SIDEWALK_C, COLOR_BLACK, COLOR_GREEN);  \
    init_pair(ROAD_C, COLOR_WHITE, COLOR_BLACK);            \
    init_pair(SIDEWALK_C, COLOR_BLACK, COLOR_YELLOW);       \
                                                            \
    init_pair(HP_FULL_SLOT, COLOR_WHITE, COLOR_GREEN);      \
    init_pair(HP_PARTIAL_SLOT, COLOR_WHITE, COLOR_YELLOW);  \
    init_pair(HP_EMPTY_SLOT, COLOR_WHITE, COLOR_RED);       \
    init_pair(SCORE_C, COLOR_YELLOW, COLOR_BLACK);          \
                                                            \
    init_pair(FROG, COLOR_GREEN, COLOR_BLACK);              \
}

#define INFO_BARS 3

void init_graphics(Screen *scrn);
void center_string(char str[], int max);

LOWCOST_INFO initial_menu(int max, int may);
void update_graphics(Board *board);

#endif