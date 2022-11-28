#ifndef FROGGER_SHORTCUTS_H
#define FROGGER_SHORTCUTS_H

#define typename(x) _Generic((x),                                       \
char: "char",                                                           \
default: "other"                                                        \
)

#define CRASH_IF_NULL(ptr)                                              \
if (!ptr)                                                               \
{                                                                       \
    perror("\nImpossibile allocare memoria!\n");                        \
    exit(-1);                                                           \
}

#define CHECK_IF_CHAR(c) !strcmp(typename(c), "char")

#define IS_NOT_PTR(type, element) (sizeof(type) - sizeof(element)) == 0

#ifndef GENERIC_ALLOCATION
#define GENERIC_ALLOCATION

#define CALLOC (char) 0
#define MALLOC (int) 0
#define REALLOC (bool) 0

#define ALLOC(name, type, size, atype)                                  \
name = (type*) _Generic((atype),                                        \
int: malloc((sizeof(type)) * (size)),                                   \
char: calloc((size), (sizeof(type))),                                   \
bool: realloc(name, sizeof(type) * size)                                \
);

#endif // GENERIC_ALLOCATION

#ifndef SPECIFIC_ALLOCATION
#define SPECIFIC_ALLOCATION

// Ignora warning superfluo per la funzioni di allocazione.
#pragma clang diagnostic ignored "-Wnon-literal-null-conversion"

// Malloc con dichiarazione
#define DMALLOC(name, type, size)                                       \
type *name = (type*) malloc(sizeof(type) * (size));                     \
CRASH_IF_NULL(name)                                                     \
if (CHECK_IF_CHAR(name[0]) && IS_NOT_PTR(char, name[0]))                \
{                                                                       \
    AREALLOC(type, name, size + 1)                                      \
    name[size] = '\0';                                                  \
}

#define DCALLOC(name, type, size)                                       \
type *name = (type*) calloc((size), sizeof(type));                      \
CRASH_IF_NULL(name)                                                     \
if (CHECK_IF_CHAR(name[0]) && IS_NOT_PTR(char, name[0]))                \
{                                                                       \
    AREALLOC(type, name, size + 1)                                      \
    name[size] = '\0';                                                  \
}

#define AREALLOC(type, arr, size)                                       \
arr = (type*) realloc(arr, sizeof(type) * (size));                      \
CRASH_IF_NULL(arr)


#define INIT_DEFAULT(type, arr, size, default_value)                    \
DMALLOC(arr, type, size)                                                \
for (int kk = 0; kk < size; kk++) arr[kk] = default_value;

#define MULTILEVEL_INIT_DEFAULT(type, arr, size1, size2, default_value) \
DMALLOC(arr, type*, size1)                                              \
for (int kk = 0; kk < size1; kk++) {                                    \
    ALLOC(arr[kk], type, size2, MALLOC)                                 \
    for (int jj = 0; jj < size2; jj++) {                                \
        arr[kk][jj] = default_value;                                    \
    }                                                                   \
}

#endif // SPECIFIC_ALLOCATION

#define PRINT_ADDR(x) printf("ADDR = %p\n", &x)
#define GET_ADDR(ptr) (long long int) &(*ptr)

#define GET_F(matrix, item) (&(*matrix)[item])
#define GET(matrix, first, second) (&(matrix)[first][second])

#define DOUBLEFREE(element, size)                                       \
for (int CC = size - 1; CC > -1; CC--) {                                \
    free(element[CC]);                                                  \
}                                                                       \
free(element);

#define SWAP_PTRS(type, x, y)                                           \
type *temp = x;                                                         \
x = y;                                                                \
y = temp;

// Converte una stringa in un numero intero.
#define TO_INT(var) strtol(var, NULL, 10)

#define NULL_INIT(var, type) type* var = NULL;

#define PRINT_MATRIX(matrix, size1, size2, format)                      \
for (int ii = 0; ii < size1; ii++) {                                    \
    for (int rr = 0; rr < size2; rr++) {                                \
        printf(format, (matrix)[ii][rr]);                               \
    }                                                                   \
    printf("\n");                                                       \
}

#endif //FROGGER_SHORTCUTS_H
