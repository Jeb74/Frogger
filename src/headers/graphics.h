
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

int display(bool *started);

#endif