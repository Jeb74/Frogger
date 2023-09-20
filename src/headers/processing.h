#ifndef FROGGER_PROCESSING_H
#define FROGGER_PROCESSING_H

#include "structures.h"

#define READ 0
#define WRITE 1
static struct timeval tv = {.tv_sec = 0, .tv_usec = 50};
#define ACCEPTABLE_WAITING_TIME &tv

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define HANDLE_ERROR(func)      \
if (func <= 0) perror("[Error Handler] Something went wrong: "TOSTRING(__LINE__)" in "TOSTRING(__FILE__)"\n")

#define CLOSE_READ(_pipe)   \
{close(_pipe.accesses[READ]);}
#define CLOSE_WRITE(_pipe)  \
{close(_pipe.accesses[WRITE]);}

#define PIPE_SIZE 2
#define PIPE_NAME 32
#define FAILED_PIPE (pipe_t){.name=NULL, .accesses={-1,-1}}

typedef struct {
    int accesses[PIPE_SIZE];
    char *name;
} pipe_t;

typedef struct {
    pid_t pid;
    char *name;
    LOWCOST_INFO status;
} Process;

LOWCOST_INFO process_mode_exec(Screen screen);
bool writeifready(void *buff, pipe_t _pipe, size_t size);
bool readifready(void *buff, pipe_t _pipe, size_t size);
void writeto(void *buff, pipe_t _pipe, size_t size);
void readfrm(void *buff, pipe_t _pipe, size_t size);
pipe_t *create_pipes(int size, ...);
pipe_t create_pipe(char name[PIPE_NAME]);
pipe_t findpn(int len, pipe_t pipes[], char name[PIPE_NAME]);

#endif