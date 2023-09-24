#include "../headers/collide.h"

#define _FROG_SPAWN_X(board) (int) board.screen_x / 2
#define _FROG_SPAWN_Y(board) board.fp.y - 1

#define _BOARD_SPACES_TOP_Y 9
#define _BOARD_SPACES_LOW_Y 2

int map_upper_bound;
int map_lower_bound;
int map_right_bound;
int map_left_bound;

int river_upper_bound;
int river_lower_bound;

/**
 * Prende la coordinata X in alto a destra dell'entita'.
 * @param entity L'entita'.
 */
int get_top_right_corner_x(Entity *entity)
{
    return entity->position.x + entity->length;
}

/**
 * Prende la coordinata Y in alto a destra dell'entita'.
 * @param entity L'entita'.
 */
int get_top_right_corner_y(Entity *entity)
{
    return entity->position.y;
}

/**
 * Calcola le coordinate dell'angolo in alto a destra dell'entita'.
 * @param entity L'entita'.
 * @return Le coordinate [x, y].
*/
int *get_top_right_corner(Entity *entity)
{
    int *corner = MALLOC(int, 2);

    corner[0] = get_top_right_corner_x();
    corner[1] = get_top_right_corner_y();

    return corner;
}

/**
 * Prende la coordinata X in alto a sinistra dell'entita'.
 * @param entity L'entita'.
 */
int get_bottom_left_corner_x(Entity *entity)
{
    return entity->position.x;
}

/**
 * Prende la coordinata Y in alto a sinistra dell'entita'.
 * @param entity L'entita'.
 */
int get_bottom_left_corner_y(Entity *entity)
{
    return entity->position.y - 1;
}

/**
 * Calcola le coordinate dell'angolo in alto a sinistra dell'entita'.
 * @param entity L'entita'.
 * @return Le coordinate [x, y].
*/
int *get_bottom_left_corner(Entity *entity)
{
    int *corner = MALLOC(int, 2);

    corner[0] = get_bottom_left_corner_x();
    corner[1] = get_bottom_left_corner_y();

    return corner;
}

/**
 * Prende la coordinata X in basso a destra dell'entita'.
 * @param entity L'entita'.
 */
int get_bottom_right_corner_x(Entity *entity)
{
    return entity->position.x + entity->length;
}

/**
 * Prende la coordinata Y in basso a destra dell'entita'.
 * @param entity L'entita'.
*/
int get_bottom_right_corner_y(Entity *entity)
{
    return entity->position.y - 1;
}

/**
 * Calcola le coordinate dell'angolo in basso a destra dell'entita'.
 * @param entity L'entita'.
 * @return Le coordinate [x, y].
 */
int *get_bottom_right_corner(Entity *entity)
{
    int *corner = MALLOC(int, 2);

    corner[0] = get_bottom_right_corner_x();
    corner[1] = get_bottom_right_corner_y();

    return corner;
}

/**
 * Prende la coordinata X in basso a sinistra della rana.
 * @param entity L'entita'.
 */
int get_frog_top_left_corner_x(Board *board)
{
    return board->fp.x;
}

/**
 * Prende la coordinata Y in basso a sinistra della rana.
 * @param entity L'entita'.
 */
int get_frog_top_left_corner_y(Board *board)
{
    return board->fp.y;
}

/**
 * Calcola le coordinate dell'angolo in basso a sinistra della rana.
 * @param entity L'entita'.
 * @return Le coordinate [x, y].
 */
int *get_frog_top_left_corner(Board *board)
{
    int *corner = MALLOC(int, 2);

    corner[0] = get_frog_top_left_corner_x(board);
    corner[1] = get_frog_top_left_corner_y(board);

    return corner;
}

/**
 * Prende la coordinata X in basso a destra della rana.
 * @param entity L'entita'.
 */
int get_frog_bottom_right_corner_x(Board *board)
{
    return board->fp.x + (FROG_WIDTH / 2);
}

/**
 * Prende la coordinata Y in basso a destra della rana.
 * @param entity L'entita'.
 */
int get_frog_bottom_right_corner_y(Board *board)
{
    return board->fp.y + (FROG_HEIGHT / 2);
}

/**
 * Calcola le coordinate dell'angolo in basso a destra della rana.
 * @param entity L'entita'.
 * @return Le coordinate [x, y].
 */
int *get_frog_bottom_right_corner(Board *board)
{
    int *corner = MALLOC(int, 2);

    corner[0] = get_frog_bottom_right_corner_x(board);
    corner[1] = get_frog_bottom_right_corner_y(board);

    return corner;
}

/**
 * Prende la coordinata X in basso a sinistra della rana.
 * @param entity L'entita'.
 */
int get_frog_bottom_left_corner_x(Board *board)
{
    return board->fp.x - (FROG_WIDTH / 2);
}

/**
 * Prende la coordinata Y in basso a sinistra della rana.
 * @param entity L'entita'.
 */
int get_frog_bottom_left_corner_y(Board *board)
{
    return board->fp.y;
}

/**
 * Calcola le coordinate dell'angolo in basso a sinistra della rana.
 * @param entity L'entita'.
 * @return Le coordinate [x, y].
*/
int *get_frog_bottom_left_corner(Board *board)
{
    int *corner = MALLOC(int, 2);

    corner[0] = get_frog_bottom_left_corner_x(board);
    corner[1] = get_frog_bottom_left_corner_y(board);

    return corner;
}

/**
 * Prende la coordinata X in alto a destra della rana.
 * @param entity L'entita'.
 */
int get_frog_top_right_corner_x(Board *board)
{
    return board->fp.x + (FROG_WIDTH / 2);
}

/**
 * Prende la coordinata Y in alto a destra della rana.
 * @param entity L'entita'.
 */
int get_frog_top_right_corner_y(Board *board)
{
    return board->fp.y;
}

/**
 * Calcola le coordinate dell'angolo in alto a destra della rana.
 * @param entity L'entita'.
 * @return Le coordinate [x, y].
 */
int *get_frog_top_right_corner(Board *board)
{
    int *corner = MALLOC(int, 2);

    corner[0] = get_frog_top_right_corner_x(board);
    corner[1] = get_frog_top_right_corner_y(board);

    return corner;
}

/**
 * Controlla se la rana e' uscita dai bordi della mappa.
 * @param board La board.
 * @return Se la rana e' uscita dai bordi della mappa.
*/
bool check_map_borders_frog(Board *board)
{
    Position *frogPosition = &board->fp;
    
    int left_x = frogPosition->x;
    int left_y = frogPosition->y;

    int right_x = left_x + 1;
    int right_y = left_y - 1;

    if (left_y > upper_bound)
    {
        frogPosition->y = map_upper_bound;
        return true;
    }

    if (right_y < lower_bound)
    {
        frogPosition->y = map_lower_bound;
        return true;
    }

    if (left_x < left_bound)
    {
        frogPosition->x = map_left_bound;
        return true;
    }

    if (right_x > right_bound)
    {
        frogPosition->x = map_right_bound;
        return true;
    }

    return false;
}

/**
 * Controlla se un tronco e' uscito dai bordi della mappa.
 * @param board La board.
 * @param log Il tronco.
 * @param exm La modalita' di esecuzione.
 * @return Se il tronco e' uscito dai bordi della mappa.
*/
bool check_map_borders_log(Board *board, Entity *log, ExecutionMode exm)
{
    int top_left_x = log->position.x;
    int bottom_right_x = get_bottom_right_corner_x(log);

    if (top_left_x < map_left_bound)
    {
        log->position.x = map_left_bound;
        return true;
    }

    if (bottom_right_x > map_right_bound)
    {
        log->position.x = map_right_bound;
        return true;
    }

    return false;
}

/**
 * Controlla se un veicolo e' uscito dai bordi della mappa.
 * @param board La board.
 * @param vehicle Il veicolo.
 * @param exm La modalita' di esecuzione.
 * @return Se il veicolo e' uscito dai bordi della mappa.
*/
bool check_borders_vehicle(Board *board, Entity *vehicle, ExecutionMode exm)
{
    int top_left_x = vehicle->position.x;
    int bottom_right_x = get_bottom_right_corner_x;

    if (vehicle->action == LEFT && top_left_x < map_left_bound)
    {
        vehicle->position.x = map_right_bound; // todo replace?
        return true;
    }

    if (vehicle->action == RIGHT && bottom_right_x > map_right_bound)
    {
        vehicle->position.x = map_left_bound; // todo replace?
        return true;
    }

    return false;
}

/**
 * Controlla se la rana e' stata uccisa da un veicolo.
 * @param board La board.
 * @param eq La coda delle entita'.
*/
bool check_frog_by_vehicle(Board *board, EntityQueue *eq)
{
    int frog_top_left_x = board->fp.x;
    int frog_bottom_right_x = get_frog_bottom_right_corner_x(board);

    EntityQueue *_eq = eq;

    while (_eq->next != NULL)
    {
        Entity *vehicle = _eq->e;

        if (vehicle->type != CAR && vehicle->type != TRUCK)
        {
            continue;
        }

        int vehicle_top_left_x = vehicle->position.x;
        int vehicle_top_right_x = get_top_right_corner_x(vehicle);

        if (vehicle->action == LEFT && frog_top_left_x == vehicle_top_right_x)
        {
            return true;
        }

        if (vehicle->action == RIGHT && vehicle_top_right_x == frog_top_left_x)
        {
            return true;
        }

        _eq = _eq->next;
    }
    
    return false;
}

/**
 * Controlla se la rana e' stata uccisa da un proiettile.
 * @param board La board.
 * @param eq La coda delle entita'.
*/
bool check_frog_by_projectile(Board *board, EntityQueue *eq)
{
    int frog_top_left_x = board->fp.x;
    int frog_top_left_y = board->fp.y;

    int frog_bottom_left_x = get_frog_bottom_left_corner_x(board);
    int frog_bottom_left_y = get_frog_bottom_left_corner_y(board);

    int frog_top_right_x = get_frog_top_right_corner_x(board);
    int frog_top_right_y = get_frog_top_right_corner_y(board);

    int frog_bottom_right_x = get_frog_bottom_right_corner_x(board);
    int frog_bottom_right_y = get_frog_bottom_right_corner_y(board);

    EntityQueue *_eq = eq;

    while (_eq->next != NULL)
    {
        Entity *projectile = _eq->e;

        if (projectile->type != PROJECTILE)
        {
            continue;
        }

        int projectile_x = projectile->position.x;
        int projectile_y = projectile->position.y;

        if (projectile_x == frog_top_left_x && projectile_y == frog_top_left_y)
        {
            return true;
        }

        if (projectile_x == frog_bottom_left_x && projectile_y == frog_bottom_left_y)
        {
            return true;
        }

        if (projectile_x == frog_top_right_x && projectile_y == frog_top_right_y)
        {
            return true;
        }

        if (projectile_x == frog_bottom_right_x && projectile_y == frog_bottom_right_y)
        {
            return true;
        }

        _eq = _eq->next;
    }
    
    return false;
}

/**
 * Controlla e forza la fisica corretta del gioco.
 * @param board La board.
 * @param eq La coda delle entita'.
 */
void check_collisions(Board *board, EntityQueue *eq)
{
    ExecutionMode exm = get_exm();
    EntityQueue *_eq = eq;

    while (_eq->next != NULL)
    {
        EntityTypes type = _eq->e->type;

        switch (type)
        {
            case FROG:
                check_borders_frog(board);
                break;
            case LOG:
                check_borders_log(board, _eq->e, exm);
                break;
            case CAR:
            case TRUCK:
                check_borders_vehicle(board, _eq->e, exm);
                break;
            default:
                break;
        }

        _eq = _eq->next;
    }

    check_frog_by_vehicle(board, eq);
    check_frog_by_projectile(board, eq);
}