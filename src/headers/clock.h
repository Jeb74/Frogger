#ifndef FROGGER_CLOCK_H
#define FROGGER_CLOCK_H

#include "structures.h"

#define CLOCK_HIT_EVERY 1

#define ALLOC_SIZE_TIME_LEFT 10
#define ALLOC_SIZE_TIME_LEFT_BAR 25

#define WRITTEN_TIME_CHARS 11
#define ALREADY_WRITTEN_CHARS 13

#define TIME_LEFT_CHAR '|'

void manage_clock(void *arg);
char *format_clock_numeric(Board *board);
char *format_clock_bar(Board *board);

#endif // FROGGER_CLOCK_H