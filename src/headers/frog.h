#ifndef FROGGER_FROG_H
#define FROGGER_FROG_H

#include "structures.h"

#define ALLOC_SIZE_LIFES_LEFT_BAR 28

#define WRITTEN_LIFES_CHARS 12
#define ALREADY_WRITTEN_CHARS 13

#define LIFES_LEFT_CHAR 'L'

char *format_lives_numeric(Board *board);
char *format_lives_bar(Board *board);
void *manage_frog(void *args);

#endif // FROGGER_FROG_H
