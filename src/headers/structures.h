#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>

#include <ncurses.h>
#include <locale.h>

#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <semaphore.h>

#ifndef FROGGER_STRUCTURES_H
#define FROGGER_STRUCTURES_H

// Richiede l'header una sola volta, per evitare errori di ridefinizione.
#pragma once

#define PIPE_SIZE 2

// Sarebbe da utilizzare in caso si abbiano kB di RAM.
typedef char LOWCOST_INFO;

/*
 * Logical data structures
 */

struct job
{
    struct job *next; /* Link field for linked list. */
    size_t id;        /* Job ID */
    void *raw_packet; /* Pointer to data */
};

struct worker_args
{
    struct job **job_queue;
    pthread_mutex_t *job_queue_mutex;
    sem_t *job_queue_count;
};

typedef enum
{
    THREAD,
    PROCESS
} ExecutionMode;

typedef ExecutionMode RequestType;

typedef enum
{
    CLOCK_PKG,
    FROG_PKG,
    ENTITY_PKG,
} ContentType;

typedef struct
{
    int accesses[PIPE_SIZE];
    char *name;
} pipe_t;

typedef struct {
    pipe_t * pipes;
    unsigned int size;
} Pipes;

typedef struct
{
    pid_t pid;
    char *name;
    LOWCOST_INFO status;
} Process;

struct t_clock_packet
{
    pthread_mutex_t *mutex;
};

struct p_clock_packet
{
    /* communicatetime, servicing, readysignal*/
    pipe_t c, se, s;
};

typedef struct
{
    union
    {
        struct t_clock_packet t;
        struct p_clock_packet p;
    } carriage;

    unsigned int *time_left;
    bool cancelled;
} ClockPacket;

/* Game data structure start */
typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    SHOOT,
    NONE,
    RQPAUSE
} Action;
/* Game data structure end */

struct t_entity_move_packet
{
    pthread_mutex_t *entity_mutex;
    Action *entity_action;
};

// <!> Forse non necessario
// struct output
// {
//     unsigned int id;
//     Action performed;
// };

struct p_entity_move_packet
{
    /* communicateaction, servicing, readysignal */
    pipe_t c, se, s;
    
    // <!> Forse non necessario
    // struct output output;
};

typedef struct
{
    union
    {
        struct t_entity_move_packet t;
        struct p_entity_move_packet p;
    } carriage;

    unsigned int id;
    bool cancelled;
} TinyEntityMovePacket;

typedef struct
{
    TinyEntityMovePacket sub_packet;
    Action default_action;
} EntityMovePacket;

typedef struct
{
    RequestType *rqtype;
    ContentType *cntype;
    union
    {
        ClockPacket *cpkt;
        TinyEntityMovePacket *tempkt;
        EntityMovePacket *empkt;
    } arg;
} Package;

/*
 * Game data structures
 */

typedef enum
{
    FROG,
    CAR,
    TRUCK,
    LOG,
    ENEMY_FROG,
    ENEMY_BIRD,
    ENEMY_SNAKE,
    PROJECTILE
} EntityTypes;

typedef struct
{
    int x;
    int y;
} Position;

typedef struct
{
    Position position;
    bool used;
} Hideout;

typedef struct
{
    unsigned char id;
    EntityTypes type;

    Position position;
    Action action;

    unsigned int length;
} Entity;

struct _equeue
{
    struct _equeue *next;
    Entity *e;
    unsigned int id;
};

typedef struct _equeue EntityQueue;

typedef struct
{
    unsigned int screen_x;
    unsigned int top_y;
    unsigned int low_y;
    bool is_game_won;

    unsigned int points;
    LOWCOST_INFO lifes_on_start;
    LOWCOST_INFO lifes_left;
    Position fp;

    unsigned int max_time;
    unsigned int time_left;
} Board;

/*
 * Graphical data structures
 */

typedef struct
{
    unsigned int y;
    unsigned int x;
} Screen;

typedef struct
{
    int value;
} Bar;

#define unpack(_pkg, buffer, rq_type, cn_type)            \
    Package *pkg = (Package *)_pkg;                       \
    bool validrq = rq_type == (*(*pkg).rqtype);           \
    bool validcn = cn_type == (*(*pkg).cntype);           \
    if (!validrq || !validcn)                             \
    {                                                     \
        perror("[Unpacking Error] Invalid request.");     \
        exit(1);                                          \
    }                                                     \
    switch (cn_type)                                      \
    {                                                     \
    case CLOCK_PKG:                                       \
        buffer = (*pkg).arg.cpkt;                         \
        break;                                            \
    case ENTITY_PKG:                                      \
        buffer = (*pkg).arg.empkt;                        \
        break;                                            \
    case FROG_PKG:                                        \
        buffer = (*pkg).arg.tempkt;                       \
        break;                                            \
    default:                                              \
        perror("[Unpacking Error] Something went wrong.");\
        break;                                            \
    }

Package *pack(RequestType rqtype, ContentType cntype, ...);
void destroy_package(Package *pkg);

#define STD_ENTITIES 15

#include "shortcuts.h"
#include "threading.h"
#include "processing.h"

#include "data.h"
#include "clock.h"
#include "frog.h"
#include "graphics.h"
#include "board.h"

#endif // FROGGER_STRUCTURES_H
