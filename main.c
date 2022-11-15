#include "utils/structures.h"
#include "utils/shortcuts.h"
#include "utils/globalmacros.h"
#include "utils/libraries.h"

int main(int argc, char **argv) {
    char *string = NULL;

    CALLOC(string21, char, 3)
    string21[0] = 'a';
    string21[1] = 'b';
    string21[2] = 'c';

    INIT_DEFAULT(char, string1, 10, 'a')

    input(&string, 2, false, "(1|10)", ">> ");

    freeIt(&string21);
    freeIt(&string21);

    return 0;
}