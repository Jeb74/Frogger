#ifndef FROGGER_SHORTCUTS_H
#define FROGGER_SHORTCUTS_H

#include "libraries.h"

#define CRASH_IF_NULL(ptr)                         \
if (!ptr)                                          \
{                                                  \
    perror("\nImpossibile allocare memoria!\n");   \
    exit(-1);                                      \
}

#define ERASE_POS ' '

#define SLEEP_MILLIS(quantity) usleep(quantity * 1000)
#define SLEEP_SECONDS(quantity) SLEEP_MILLIS(quantity * 1000)

/*
 * Macro per allocazione e ri-allocazione della memoria.
 */
#define MALLOC(type, size) (type *) malloc(sizeof(type) * (size))
#define CALLOC(type, size) (type *) calloc((size), sizeof(type))
#define REALLOC(type, arr, size) (type *) realloc(arr, sizeof(type) * (size))

#define FREE_AFTER_USE_PTR(ptr, func) \
    {                                 \
        func                          \
        free(ptr);                    \
    }

#define FREE_AFTER_USE(type, size, ptr, func)   \
    {                                           \
        type *ptr = MALLOC(type, size);         \
        CRASH_IF_NULL(ptr);                     \
        FREE_AFTER_USE_PTR(ptr, func)           \
    }

#define LAMBDA_ALLOCATION(type, size, ptr, func) \
    {                                            \
        type *ptr = MALLOC(type, size);          \
        CRASH_IF_NULL(ptr);                      \
        func                                     \
    }

char get_entity_symbol(EntityTypes type);

#endif //FROGGER_SHORTCUTS_H