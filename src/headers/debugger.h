#ifndef FROGGER_DEBUGGER_H
#define FROGGER_DEBUGGER_H

#include "structures.h"

#define DEBUG 0
#define INFO 1
#define ERROR 2

static int log_level = DEBUG;
static FILE *log_file = NULL;

void init_debugger(const char *log_filename);
void set_log_level(int level);
void log_message(int level, const char *format, ...);
void close_debugger();

#endif