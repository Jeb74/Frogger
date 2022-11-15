#ifndef FROGGER_SHORTCUTS_H
#define FROGGER_SHORTCUTS_H

#define typename(x) _Generic((x),   \
char: "char",                       \
default: "other"                    \
)

#define CRASH_IF_NULL(ptr)                          \
if (ptr == NULL)                                    \
{                                                   \
    printf("\nImpossibile allocare memoria!\n");    \
    exit(-1);                                       \
}

#define CHECK_IF_CHAR(c) strcmp(typename(c), "char") == 0

#define MALLOC(name, type, size)                            \
type *name = (type *) malloc(sizeof(type) * (size));        \
CRASH_IF_NULL(name)                                         \
if (CHECK_IF_CHAR(name[0]))                                 \
{                                                           \
    REALLOC(char, name, size + 1)                           \
    name[size] = '\0';                                      \
}

#define CALLOC(name, type, size)                            \
type *name = (type *) calloc((size), sizeof(type));         \
CRASH_IF_NULL(name)                                         \
if (CHECK_IF_CHAR(name[0]))                                 \
{                                                           \
    REALLOC(char, name, size + 1)                           \
    name[size] = '\0';                                      \
}

#define REALLOC(type, arr, size)                    \
arr = (type *) realloc(arr, sizeof(type) * (size)); \
CRASH_IF_NULL(arr)

#define INIT_DEFAULT(type, arr, size, default_value)        \
MALLOC(arr, type, size)                                     \
for (int kk = 0; kk < size; kk++) arr[kk] = default_value;

#define ADDR(x) printf("ADDR = %p\n", x);

#define MULFREE(element, size)                      \
for (int CC = size - 1; CC > -1; CC--) {            \
    free(element[CC]);                              \
}                                                   \
free(element);

#define SWAP_PTRS(type, x, y)   \
type temp = *x;                 \
*x = *y;                        \
*y = temp;

// Converte una stringa in un numero intero.
#define TO_INT(var) strtol(var, NULL, 10)

#endif //FROGGER_SHORTCUTS_H
