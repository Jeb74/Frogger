#ifndef FROGGER_PROCESSING_H
#define FROGGER_PROCESSING_H

#include "structures.h"

#define KILL_SIGNAL 2
#define PAUSE_SIGNAL 69
#define RESUME_SIGNAL 70

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

#define PIPE_NAME 32
#define FAILED_PIPE (pipe_t){.name=NULL, .accesses={-1,-1}}
#define PAS 5 // pipe array size
#define MIN_PAS 2

#define TIME_COMMS "comms_time"
#define USER_COMMS "comms_action"
#define READY_PIPE "ready_signal"
#define SERVICE_TIME "service_time"
#define SERVICE_USER "service_action"

#define ENTITY_COMMS(indx) "comms_e_"STRINGIFY(indx)
#define ENTITY_SERVICES(indx) "service_e_"TOSTRING(indx)

#define OUT_OF_TIME 100

void process_mode_exec(Screen screen, WINDOW **ws);

bool writeifready(void *buff, pipe_t _pipe, size_t size);
bool readifready(void *buff, pipe_t _pipe, size_t size);

void writeto(void *buff, pipe_t _pipe, size_t size);
void readfrm(void *buff, pipe_t _pipe, size_t size);

pipe_t *create_pipes(int size, ...);
pipe_t create_pipe(char name[PIPE_NAME]);
pipe_t findpn(int len, pipe_t pipes[], char name[PIPE_NAME]);
Process palloc(char pname[PIPE_NAME], void (*_func)(void*), Package *pkg);

#endif