#ifndef FROGGER_CLOCK_H
#define FROGGER_CLOCK_H

#ifndef FROGGER_STRUCTURES_H
#include "structures.h"
#endif // FROGGER_STRUCTURES_H

#define CLOCK_HIT_EVERY 1
#define MSG_SIZE_TIME_LEFT 22
#define MSG_SIZE_TIME_LEFT_BAR 26
#define TIME_LEFT_CHAR '|'

void *manage_clock(void *arg);
char *format_clock(Board *board);
char *format_clock_bar(Board *board);

#endif // FROGGER_CLOCK_H