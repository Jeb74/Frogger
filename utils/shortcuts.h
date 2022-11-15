#ifndef FROGGER_SHORTCUTS_H
#define FROGGER_SHORTCUTS_H

#define CRASH_IF_NULL(ptr)                          \
if (ptr == NULL)                                    \
{                                                   \
    printf("\nImpossibile allocare memoria!\n");    \
    exit(-1);                                       \
}

#define MALLOC(type, size) (type *) malloc(sizeof(type) * (size))
#define CALLOC(type, size) (type *) calloc((size), sizeof(type))
#define REALLOC(type, arr, size) (type *) realloc(arr, sizeof(type) * (size))

#define ADDR(x) printf("ADDR = %p\n", x);

#define MULFREE(element, size)                      \
for (int CC = size - 1; CC > -1; CC--) {            \
    free(element[CC]);                              \
}                                                   \
free(element);

#endif //FROGGER_SHORTCUTS_H
