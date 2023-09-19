#ifndef FROGGER_SHORTCUTS_H
#define FROGGER_SHORTCUTS_H

#include "structures.h"

#define CRASH_IF_NULL(ptr)                           \
    if (!ptr)                                        \
    {                                                \
        perror("\nImpossibile allocare memoria!\n"); \
        exit(-1);                                    \
    }

#define TERM 1

#define NULL_CHAR ' '

#define EMPTY_CHAR '0'
#define FILL_CHAR '1'

#define SLEEP_MILLIS(quantity) usleep(quantity * 1000)
#define SLEEP_SECONDS(quantity) SLEEP_MILLIS(quantity * 1000)

/*
 * Macro per allocazione e ri-allocazione della memoria.
 */
#define MALLOC(type, size) (type *)malloc(sizeof(type) * (size))
#define CALLOC(type, size) (type *)calloc((size), sizeof(type))
#define REALLOC(type, arr, size) (type *)realloc(arr, sizeof(type) * (size))

#define MALLOC_TERM(type, size) (type *)malloc(sizeof(type) * (size + TERM))
#define CALLOC_TERM(type, size) (type *)calloc((size + TERM), sizeof(type))
#define REALLOC_TERM(type, arr, size) (type *)realloc(arr, sizeof(type) * (size + TERM))

#define LAMBDA_FREE_PTR(ptr, func) \
    {                              \
        func                       \
            free(ptr);             \
    }

#define LAMBDA_PTR(type, size, ptr, func) \
    {                                     \
        type *ptr = MALLOC(type, size);   \
        CRASH_IF_NULL(ptr);               \
        LAMBDA_FREE_PTR(ptr, func)        \
    }

#define LAMBDA_ALLOCATION(type, size, ptr, func) \
    {                                            \
        type *ptr = MALLOC(type, size);          \
        CRASH_IF_NULL(ptr);                      \
        func                                     \
    }

#define FREE_ALL(ptrs, size)          \
    for (size_t i = 0; i < size; i++) \
    {                                 \
        free(ptrs[i]);                \
    }                                 \
    free(ptrs)

LOWCOST_INFO getAction(int argc, char *argv[]);
unsigned int calwidth(unsigned int width, unsigned int panels, LOWCOST_INFO indx);

char get_entity_symbol(EntityTypes type);
void end_game(Board *board);

int count_digits(int value);
int divide_if_possible(int dividend, int divisor);

Bar create_bar(Board *board, int max, int current);
Bar create_life_bar(Board *board);
Bar create_time_bar(Board *board);

void calculate_bar(Bar *bar, int max, int current);
void calculate_life_bar(Bar *bar, Board *board);
void calculate_time_bar(Bar *bar, Board *board);
char **format_number(int number, char empty, char fill);
char *num_to_string(int num, unsigned int size, bool fill);
char *build_string(const char *__restrict_arr format, ...);

#endif // FROGGER_SHORTCUTS_H