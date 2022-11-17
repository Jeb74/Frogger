#ifndef FROGGER_GAME_MANAGER_H
#define FROGGER_GAME_MANAGER_H

#ifndef FROGGER_GENERAL_H
#include "../utils/general.h"
#endif

/*
 * Dimensioni dello schermo.
 */
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Numero di vite della rana.
#define FROG_LIVES 3

/*
 * Dimensioni della rana.
 */
#define FROG_HEIGHT 2
#define FROG_WIDTH 2

/*
 * Spazio massimo di salto della rana.
 */
#define FROG_MOVE_X 2
#define FROG_MOVE_Y 2

/*
 * Dimensioni delle macchine.
 */
#define AUTO_MIN_SIZE 4
#define AUTO_MAX_SIZE 6

/*
 * Dimensioni dei camion.
 */
#define CAMION_MIN_SIZE 6
#define CAMION_MAX_SIZE 10

// Calcolo dell'area.
#define CALC_AREA(x, y) (x * y)

/*
 * Numero di corsie dell'autostrada.
 */
#define HIGHWAY_MIN_LANES 3
#define HIGHWAY_MAX_LANES 5

// Altezza di una corsia dell'autostrada.
#define HIGHWAY_MIN_HEIGHT FROG_HEIGHT

/*
 * Dimensioni del prato.
 */
#define LAWN_HEIGHT FROG_HEIGHT
#define LAWN_WIDTH SCREEN_WIDTH // TODO: da cambiare

/*
 * Dimensioni del fiume.
 */
#define RIVER_MIN_LANES 3
#define RIVER_MAX_LANES 5

// Altezza di una corsia del fiume.
#define RIVER_MIN_HEIGHT FROG_SIZE

/*
 * Dimensioni della tana.
 */
#define LAIR_HEIGHT FROG_HEIGHT
#define LAIR_WIDTH FROG_WIDTH

// Il numero di tane.
#define LAIR_NUM 5

#define BOARD_HEIGHT 40
#define BOARD_WIDTH 30

enum coordinate {
    X,
    Y
};

typedef struct entity_t
{
    enum entity_types type;
    int width;
    attribute *attributes;
} *entity;

typedef union {
    entity _entity;
    enum frog_statuses frog_status;
} attribute;

struct cell
{
    entity _entity;
};

typedef struct
{
    int **map;
} board;

void init_game();

#endif //FROGGER_GAME_MANAGER_H
