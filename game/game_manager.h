#ifndef FROGGER_GAME_MANAGER_H
#define FROGGER_GAME_MANAGER_H

#ifndef FROGGER_GENERAL_H
#include "../utils/general.h"
#endif

#include <stdbool.h>

/*
 *      COMPARTO GRAFICO
 */

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
#define D_FROG_HEIGHT 2
#define D_FROG_WIDTH 2

/*
 * Spazio massimo di salto della rana.
 */
#define D_FROG_MOVE_X 2
#define D_FROG_MOVE_Y 2

/*
 * Dimensioni delle macchine.
 */
#define D_AUTO_MIN_SIZE 4
#define D_AUTO_MAX_SIZE 6

/*
 * Dimensioni dei camion.
 */
#define D_CAMION_MIN_SIZE 6
#define D_CAMION_MAX_SIZE 10

// Calcolo dell'area.
#define CALC_AREA(x, y) (x * y)

/*
 * Numero di corsie dell'autostrada.
 */
#define D_HIGHWAY_MIN_LANES 3
#define D_HIGHWAY_MAX_LANES 5

// Altezza di una corsia dell'autostrada.
#define D_HIGHWAY_MIN_HEIGHT D_FROG_HEIGHT
#define D_HIGHWAY_HEIGHT D_HIGHWAY_MAX_LANES

/*
 * Dimensioni del prato.
 */
#define D_LAWN_HEIGHT D_FROG_HEIGHT
#define D_LAWN_WIDTH SCREEN_WIDTH // TODO: da cambiare

/*
 * Dimensioni del fiume.
 */
#define D_RIVER_MIN_LANES 3
#define D_RIVER_MAX_LANES 5

// Altezza di una corsia del fiume.
#define D_RIVER_MIN_HEIGHT D_FROG_SIZE

/*
 * Dimensioni della tana.
 */

#define D_DEN_HEIGHT D_FROG_HEIGHT
#define D_DEN_WIDTH D_FROG_WIDTH

// Il numero di tane.
#define D_DEN_NUM R_DEN_NUM

// Dimensioni della board
#define D_BOARD_HEIGHT 26
#define D_BOARD_WIDTH 13

/*
 *      COMPARTO DATA
 */

#define R_FROG_HEIGHT 1
#define R_FROG_WIDTH 1

#define R_BOARD_HEIGHT 13
#define R_BOARD_WIDTH 15

#define R_RIVER_LANES D_RIVER_MAX_LANES

#define R_HIGHWAY_LANES D_HIGHWAY_MAX_LANES

#define R_DEN_NUM 5

enum coordinate {
    X,
    Y
};

typedef struct
{
    enum entity_types type;
    short width;
    short curr_width;
    union {
        void *entity;  // nemico/rana
        enum frog_statuses *frog_status;
    } attributes;
} entity;

enum row_types {
    WALKABLE = 0,
    HIGHWAY = R_HIGHWAY_LANES,
    LAWN = HIGHWAY + R_FROG_HEIGHT,
    RIVER = LAWN + R_RIVER_LANES,
    DEN = RIVER + R_FROG_HEIGHT
};

typedef struct
{
    entity *_entity;
    struct projectile *proj;
} cell;

enum directions {
    right,
    left,
    up,
    down
};

typedef struct
{
    cell **cells;

    struct {
        enum directions direction;
        enum row_types type;
    } specifics[R_BOARD_HEIGHT];
} board;

enum move_e {
    LINE_T,
    FROG_T,
    PROJECTILE_T
};

void init_game();

#endif //FROGGER_GAME_MANAGER_H
