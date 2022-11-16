#include "utils/general.h"

int main(int argc, char **argv) {
    char *var = NULL;
    input(&var, 8, true, "(A|Z)(a|z)(^0|^9)", ">>");
    printf("%s\n", var);
    var = resizeStr(var, 5, RIGHT);
    printf("%s\n", var);
    return 0;
}