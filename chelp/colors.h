#ifndef FROGGER_COLORS_H
#define FROGGER_COLORS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#if defined WIN32 || defined WIN64
#include <windows.h>
#elif defined __linux__  || defined __APPLE__
#include <unistd.h>
#endif
extern enum {
    RESET,
    BLUE,
    GREEN,
    CYAN,
    RED,
    PURPLE,
    YELLOW,
    WHITE,
    BLACK
} colors;
#define COLOR_INTENSITY 8
#define NORMAL_COLOR_CAP 16
#if defined __linux__ || defined __APPLE__
#define COLOR_RESET "\033[0m"
#define MAX_COLOR_STRING_LEN 13
#define BACKGROUND 'B'
#define FOREGROUND 'F'
#define ASCII_END_STRING '\0'
#define BF_SEPARATOR ';'
#define RESET_COLOR '0'
#define COLOR_ROOT "\033["
#define COLOR_WRAPPER_ACCESS '['
#define B_BLACK "40"
#define F_BLACK "30"
#define B_BLUE "44"
#define F_BLUE "34"
#define B_GREEN "42"
#define F_GREEN "32"
#define B_CYAN "46"
#define F_CYAN "36"
#define B_RED "41"
#define F_RED "31"
#define B_PURPLE "45"
#define F_PURPLE "35"
#define B_YELLOW "43"
#define F_YELLOW "33"
#define B_WHITE "47"
#define F_WHITE "37"
#define COLOR_INTENSITY_DIGIT_O '1'
#define COLOR_INTENSITY_DIGIT_T '0'
#define COLOR_INTENSITY_DIGIT_TH '9'
#define SKIP 2
#define COLOR_WRAPPER_EXIT 'm'
#endif

void coloredText(char str[], unsigned int foreground, unsigned int background);

#endif //FROGGER_COLORS_H
