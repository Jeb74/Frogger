#include "colors.h"

// Questa funzione viene chiamata solo nel caso in cui il sistema operativo sia Linux o MacOS
// essa ha lo scopo di convertire il valore numerico del colore in stringa.
#if defined __linux__  || defined __APPLE__
static void colorCodeConverter(char **convertedColor, unsigned int color, char BorF) {
    unsigned int defaultColor = (color > COLOR_INTENSITY && color < NORMAL_COLOR_CAP) ? color - COLOR_INTENSITY : color;
    switch(defaultColor) {
        case BLACK: (BorF == BACKGROUND) ? strcat(*convertedColor, B_BLACK) : strcat(*convertedColor, F_BLACK);
            break;
        case BLUE: (BorF == BACKGROUND) ? strcat(*convertedColor, B_BLUE) : strcat(*convertedColor, F_BLUE);
            break;
        case GREEN: (BorF == BACKGROUND) ? strcat(*convertedColor, B_GREEN) : strcat(*convertedColor, F_GREEN);
            break;
        case CYAN: (BorF == BACKGROUND) ? strcat(*convertedColor, B_CYAN) : strcat(*convertedColor, F_CYAN);
            break;
        case RED: (BorF == BACKGROUND) ? strcat(*convertedColor, B_RED) : strcat(*convertedColor, F_RED);
            break;
        case PURPLE: (BorF == BACKGROUND) ? strcat(*convertedColor, B_PURPLE) : strcat(*convertedColor, F_PURPLE);
            break;
        case YELLOW: (BorF == BACKGROUND) ? strcat(*convertedColor, B_YELLOW) : strcat(*convertedColor, F_YELLOW);
            break;
        case WHITE: (BorF == BACKGROUND) ? strcat(*convertedColor, B_WHITE) : strcat(*convertedColor, F_WHITE);
            break;
    }
    if (BorF == BACKGROUND && color > COLOR_INTENSITY) {
        _Bool breaks = false;
        int i = 0;
        do {
            if ((*convertedColor)[i] == COLOR_WRAPPER_ACCESS) {
                i++;
                (*convertedColor)[i] = COLOR_INTENSITY_DIGIT_O;
                i++;
                (*convertedColor)[i+1] = (*convertedColor)[i];
                (*convertedColor)[i] = COLOR_INTENSITY_DIGIT_T;
                i+=SKIP;
                (*convertedColor)[i] = BF_SEPARATOR;
                breaks = true;
            }
            else i++;
        } while(!breaks);

    }
    else if (color > COLOR_INTENSITY && BorF == FOREGROUND) {
        int i = 0;
        _Bool breaks = false;
        do {
            if ((*convertedColor)[i] == BF_SEPARATOR) {
                i++;
                (*convertedColor)[i] = COLOR_INTENSITY_DIGIT_TH;
                i+=SKIP;
                (*convertedColor)[i] = COLOR_WRAPPER_EXIT;
                breaks = true;
            }
            else i++;
        } while (!breaks);
    }
    else if ( BorF == BACKGROUND && color <= COLOR_INTENSITY) {
        (*convertedColor)[strlen(*convertedColor)] = BF_SEPARATOR;
    }
    else {
        (*convertedColor)[strlen(*convertedColor)] = COLOR_WRAPPER_EXIT;
    }
}
#endif

/*
 * DO NOT INTENSIFY BLACK COLOR!
 */

// Questa funzione è composta da due parti, una abilitata nel sistema operativo windows e l'altra nei sistemi UNIX-like
void coloredText(char str[], unsigned int foreground, unsigned int background) {
    static const unsigned int backgroundInterval = NORMAL_COLOR_CAP;
#if defined WIN32 || defined WIN64
    background *= backgroundInterval;
    HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(Console, foreground | background);
    printf("%s", str);
    SetConsoleTextAttribute(Console, RESET);
#elif defined __linux__  || defined __APPLE__
    // Ricostruisco la stringa da stampare con i codici dei colori da applicargli.
    const static char reset[] = COLOR_RESET;
    char *color = (char *) calloc(MAX_COLOR_STRING_LEN + strlen(str) +  strlen(reset), sizeof(char));
    color = strcpy(color, COLOR_ROOT);
    if (background != RESET) colorCodeConverter(&color, background, BACKGROUND);
    else {
        _Bool breaks = false;
        for (int i = 0; !breaks; i++) {
            if (color[i] == COLOR_WRAPPER_ACCESS) {
                i++;
                color[i] = RESET_COLOR;
                i++;
                color[i] = BF_SEPARATOR;
                breaks = true;
            }
        }
    }
    colorCodeConverter(&color, foreground, FOREGROUND);
    strcat(color, str);
    strcat(color, reset);
    color = (char *) realloc(color, sizeof(char) * (strlen(color) + 1));
    color[strlen(color)] = ASCII_END_STRING;
    printf("%s", color);
    free(color);
#endif
}

/*
TEXT:
#define BLK "\e[0;30m" -> 0 + 8
#define RED "\e[0;31m" -> 4
#define GRN "\e[0;32m" -> 2
#define YEL "\e[0;33m" -> 6
#define BLU "\e[0;34m" -> 1
#define MAG "\e[0;35m" -> 5
#define CYN "\e[0;36m" -> 3
#define WHT "\e[0;37m" -> 7

BACKGROUND:
#define BLKB "\e[40m" -> (0 + 8) * 16
#define REDB "\e[41m" -> 4 * 16
#define GRNB "\e[42m" -> 2 * 16
#define YELB "\e[43m" -> 6 * 16
#define BLUB "\e[44m" -> 1 * 16
#define MAGB "\e[45m" -> 5 * 16
#define CYNB "\e[46m" -> 3 * 16
#define WHTB "\e[47m" -> 7 * 16

INTENSITY TEXT:
#define HBLK "\e[0;90m" -> 8
#define HRED "\e[0;91m" -> 4 * 8
#define HGRN "\e[0;92m" -> 2 * 8
#define HYEL "\e[0;93m" -> 6 * 8
#define HBLU "\e[0;94m" -> 1 * 8
#define HMAG "\e[0;95m" -> 5 * 8
#define HCYN "\e[0;96m" -> 3 * 8
#define HWHT "\e[0;97m" -> 7 * 8

INTENSITY BACKGROUND:
#define BLKHB "\e[0;100m" -> 8 * 16
#define REDHB "\e[0;101m" -> (4 + 8) * 16
#define GRNHB "\e[0;102m" -> (2 + 8) * 16
#define YELHB "\e[0;103m" -> (6 + 8) * 16
#define BLUHB "\e[0;104m" -> (1 + 8) * 16
#define MAGHB "\e[0;105m" -> (5 + 8) * 16
#define CYNHB "\e[0;106m" -> (3 + 8) * 16
#define WHTHB "\e[0;107m" -> (7 + 8) * 16
 */