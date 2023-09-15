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

#ifndef FROGGER_STRUCTURES_H
#define FROGGER_STRUCTURES_H

#ifndef FROGGER_STRUCTURES_H
#include "structures.h"
#endif // FROGGER_STRUCTURES_H

#pragma once

/*
Shit lowcost
*/

typedef unsigned char LOWCOST_INFO;

static pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;

/*
Game data structures
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

typedef enum
{
    NONE,
    LEFT,
    RIGHT,
    UP,
    DOWN
} Direction;

typedef struct
{
    int x;
    int y;
} Position;

typedef struct
{
    int x;
    int y;
} Velocity;

typedef struct
{
    int x;
    int y;
} Size;

typedef struct
{
    EntityTypes type;
    long long *id;

    Position position;
    Direction direction;
    Velocity velocity;

    Size size;
} Entity;

typedef struct
{
    Position position;
    bool used;
    Size size;
} Hideout;

typedef struct
{
    int MAX_X;
    int MAX_Y;
    bool is_game_won;

    int points;
    int lives_left;

    unsigned int max_time;
    unsigned int time_left;
} Board;

typedef struct
{
    Board *board;
    Entity *entity;
} GameArgs;

typedef struct
{
    Entity *e;
} Entity_Map;

typedef enum
{
    THREAD,
    PROCESS
} RequestType;
typedef enum
{
    GAMEPKG,
    GENPKG
} ContentType;

typedef struct
{
    RequestType *rqtype;
    ContentType *cntype;
    union
    {
        GameArgs *targ;
        void *garg;
    } arg;
} Package;

#define unpack(_pkg, buffer, rq_type, cn_type)        \
    Package *pkg = (Package *) _pkg;                  \
    bool validrq = rq_type == (*(*pkg).rqtype);       \
    bool validcn = cn_type == (*(*pkg).cntype);       \
    if (!validrq || !validcn)                         \
    {                                                 \
        perror("[Unpacking Error] Invalid request."); \
        exit(1);                                      \
    }                                                 \
    if (cn_type == GAMEPKG)                           \
        buffer = (GameArgs *) (*pkg).arg.targ;        \
    else                                              \
        buffer = (*pkg).arg.garg

Package *pack(RequestType rqtype, ContentType cntype, void *arg);

#include "shortcuts.h"

#include "threading.h"

#include "data.h"
#include "clock.h"
#include "frog.h"
#include "graphics.h"
#include "board.h"

#endif // FROGGER_STRUCTURES_H
