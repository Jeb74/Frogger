#ifndef FROGGER_CLOCK_H
#define FROGGER_CLOCK_H

#include "structures.h"

// Il numero di secondi per lo scorrere del tempo.
#define CLOCK_HIT_EVERY 1

void *manage_clock(void *arg);
char *format_clock_numeric(Board *board);

#endif // FROGGER_CLOCK_H