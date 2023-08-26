#ifndef FROGGER_STRUCTURES_H
#define FROGGER_STRUCTURES_H

#include "libraries.h"

static pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;

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
    NONE, LEFT, RIGHT, UP, DOWN
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
    char symbol;

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
    int time_left;
} Board;

typedef struct
{
    Board *board;
    Entity *entity;
} ThreadArgs;

#endif //FROGGER_STRUCTURES_H
