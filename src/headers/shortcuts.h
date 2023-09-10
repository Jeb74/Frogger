#ifndef FROGGER_SHORTCUTS_H
#define FROGGER_SHORTCUTS_H

#include "libraries.h"

#define CRASH_IF_NULL(ptr)                                              \
if (!ptr)                                                               \
{                                                                       \
    perror("\nImpossibile allocare memoria!\n");                        \
    exit(-1);                                                           \
}

#define ERASE_POS ' '

#define SLEEP_MILLIS(quantity) usleep(quantity * 1000)
#define SLEEP_SECONDS(quantity) SLEEP_MILLIS(quantity * 1000)

char get_entity_symbol(EntityTypes type);

#endif //FROGGER_SHORTCUTS_H