#include "utils/structures.h"
#include "utils/shortcuts.h"
#include "utils/globalmacros.h"

int main(int argc, char **argv) {
    char *string = NULL;
    input(&string, 2, false, "(1|10)", ">> ");
    return 0;
}