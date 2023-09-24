#ifndef FROGGER_SHORTCUTS_H
#define FROGGER_SHORTCUTS_H

#include "structures.h"

// Il valore di terminatore di stringa. ("\0")
#define TERM 1

/*
 * Utilizzati per l'encoding di dati.
 */

#define EMPTY_CHAR '0'
#define FILL_CHAR '1'
#define NULL_CHAR ' '

/*
 * Shortcuts per la funzione sleep.
 */

// Esegue lo sleep per un numero di microsecondi.
#define SLEEP_MICROS(quantity) usleep(quantity)
// Esegue lo sleep per un numero di millisecondi.
#define SLEEP_MILLIS(quantity) usleep(quantity * 1000)
// Esegue lo sleep per un numero di secondi.
#define SLEEP_SECONDS(quantity) sleep(quantity)

/*
 * Macro per allocazione e ri-allocazione della memoria.
 */

// Alloca la memoria (malloc()).
#define MALLOC(type, size) (type *)malloc(sizeof(type) * (size))
// Alloca la memoria (calloc()).
#define CALLOC(type, size) (type *)calloc((size), sizeof(type))
// Rialloca la memoria (realloc()).
#define REALLOC(type, arr, size) (type *)realloc(arr, sizeof(type) * (size))

/*
 * Macro per allocazione e ri-allocazione della memoria con terminatore (+1).
 */

// Alloca la memoria (malloc()) con terminatore.
#define MALLOC_TERM(type, size) (type *)malloc(sizeof(type) * (size + TERM))
// Alloca la memoria (calloc()) con terminatore.
#define CALLOC_TERM(type, size) (type *)calloc((size + TERM), sizeof(type))
// Rialloca la memoria (realloc()) con terminatore.
#define REALLOC_TERM(type, arr, size) (type *)realloc(arr, (sizeof(type) * size) + TERM)

/*
 * Macro per la gestione dei mutex.
 */

// Aquisisce il lock di un mutex.
#define AQUIRE_LOCK(mutex_ptr) pthread_mutex_lock(mutex_ptr)
// Rilascia il lock di un mutex.
#define RELEASE_LOCK(mutex_ptr) pthread_mutex_unlock(mutex_ptr)

#define EXEC_WHILE_LOCKED(mutex_ptr, func) \
    {                                      \
        AQUIRE_LOCK(mutex_ptr);            \
        func;                              \
        RELEASE_LOCK(mutex_ptr);           \
    }

#define EXEC_WHILE_LOCKED_IF_THREAD(mutex_ptr, func) \
    {                                                \
        if (get_exm() == THREAD)                     \
        {                                            \
            AQUIRE_LOCK(mutex_ptr);                  \
            func;                                    \
            RELEASE_LOCK(mutex_ptr);                 \
        }                                            \
        else                                         \
        {                                            \
            func;                                    \
        }                                            \
    }

// Controlla se il puntatore e' valido, altrimenti, termina il programma per mancata allocazione di memoria.
#define CRASH_IF_NULL(ptr)                           \
    if (!ptr)                                        \
    {                                                \
        perror("\nImpossibile allocare memoria!\n"); \
        exit(-1);                                    \
    }

/*
 * Madness.
 */

// Dopo aver eseguito la funzione, libera il puntatore.
#define LAMBDA_FREE_PTR(ptr, func) \
    {                              \
        func                       \
        free(ptr);                 \
    }

// Alloca il puntatore, esegue la funzione, libera il puntatore.
#define LAMBDA_PTR(type, size, ptr, func) \
    {                                     \
        type *ptr = MALLOC(type, size);   \
        CRASH_IF_NULL(ptr);               \
        LAMBDA_FREE_PTR(ptr, func)        \
    }

// Alloca il puntatore, esegue la funzione.
#define LAMBDA_ALLOCATION(type, size, ptr, func) \
    {                                            \
        type *ptr = MALLOC(type, size);          \
        CRASH_IF_NULL(ptr);                      \
        func                                     \
    }

// Libera tutti i puntatori.
#define FREE_ALL(ptrs, size)          \
    for (size_t i = 0; i < size; i++) \
    {                                 \
        free(ptrs[i]);                \
    }                                 \
    free(ptrs)

int gen_num(int min, int max);

LOWCOST_INFO get_action(int argc, char *argv[]);
unsigned int calwidth(unsigned int width, unsigned int panels, LOWCOST_INFO indx);

void end_game(Board *board);

int count_digits(int value);
int divide_if_possible(int dividend, int divisor);

Bar create_bar(int max, int current);
void calculate_bar(Bar *bar, int max, int current);

Bar create_score_bar(Board *board);
Bar create_life_bar(Board *board);
Bar create_time_bar(Board *board);

void calculate_score_bar(Bar *bar, Board *board);
void calculate_life_bar(Bar *bar, Board *board);
void calculate_time_bar(Bar *bar, Board *board);

char **format_number(int number, char empty[], char fill[]);
char *num_to_string(int num, int size);
char *build_string(const char *__restrict_arr format, ...);

#endif // FROGGER_SHORTCUTS_H