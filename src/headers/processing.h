#ifndef FROGGER_PROCESSING_H
#define FROGGER_PROCESSING_H

#include "structures.h"

#define READ 0
#define WRITE 1
static struct timeval  tv = {.tv_sec = 0, .tv_usec = 50};
#define ACCEPTABLE_WAITING_TIME &tv

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define HANDLE_ERROR(func)      \
if (func <= 0) perror("[Error Handler] Something went wrong: "TOSTRING(__LINE__)" in "TOSTRING(__FILE__))


LOWCOST_INFO process_mode_exec(Screen screen);

#endif