
#ifndef FROGGER_GRAPHICS_C
#define FROGGER_GRAPHICS_C

#ifndef FROGGER_LIBRARIES_C
#include "libraries.h"
#endif

#define SELECTED_FIRST                              \
attron(COLOR_PAIR(2) | A_BOLD);                     \
center_string("> Start threads version\n", max);    \
attroff(COLOR_PAIR(2) | A_BOLD);                    \
attron(COLOR_PAIR(1));                              \
center_string("Start processes version\n", max);    \
attroff(COLOR_PAIR(1));                             \
attron(COLOR_PAIR(3));                              \
center_string("Quit\n", max);                       \
attroff(COLOR_PAIR(3));                            

#define SELECTED_SECOND                             \
attron(COLOR_PAIR(1));                              \
center_string("Start threads version\n", max);      \
attroff(COLOR_PAIR(1));                             \
attron(COLOR_PAIR(2) | A_BOLD);                     \
center_string("> Start processes version\n", max);  \
attroff(COLOR_PAIR(2) | A_BOLD);                    \
attron(COLOR_PAIR(3));                              \
center_string("Quit\n", max);                       \
attroff(COLOR_PAIR(3));                             \

#define SELECTED_THIRD                              \
attron(COLOR_PAIR(1));                              \
center_string("Start threads version\n", max);      \
attroff(COLOR_PAIR(1));                             \
attron(COLOR_PAIR(1));                              \
center_string("Start processes version\n", max);    \
attroff(COLOR_PAIR(1));                             \
attron(COLOR_PAIR(2) | A_BOLD);                     \
center_string("> Quit\n", max);                     \
attroff(COLOR_PAIR(2) | A_BOLD);                    \

#define HIDEOUTS 2
#define RIVER 7
#define GRASS_SIDEWALK 8
#define ROAD 13
#define SIDEWALK 14

#define HIDEOUTS_C 1
#define RIVER_C 2
#define GRASS_SIDEWALK_C 3
#define ROAD_C 4
#define SIDEWALK_C 5

LOWCOST_INFO display(int max, int may);
void display_board(Board *board);

#endif