#include "../headers/collide.h"

#define _FROG_SPAWN_X(board) (int) board.screen_x / 2
#define _FROG_SPAWN_Y(board) board.fp.y - 1

#define _BOARD_SPACES_TOP_Y 9
#define _BOARD_SPACES_LOW_Y 2

int upper_bound;
int lower_bound;

int right_bound;
int left_bound;

/**
 * Controlla se la rana e' uscita dai bordi della mappa.
 * @param board La board.
 * @return Se la rana e' uscita dai bordi della mappa.
*/
bool check_borders_frog(Board *board)
{
    Position *frogPosition = &board->fp;
    
    int left_x = frogPosition->x;
    int left_y = frogPosition->y;

    int right_x = left_x + 1;
    int right_y = left_y - 1;

    if (left_y > upper_bound)
    {
        frogPosition->y = upper_bound;
        return true;
    }

    if (right_y < lower_bound)
    {
        frogPosition->y = lower_bound;
        return true;
    }

    if (left_x < left_bound)
    {
        frogPosition->x = left_bound;
        return true;
    }

    if (right_x > right_bound)
    {
        frogPosition->x = right_bound;
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
bool check_borders_log(Board *board, Entity *log, ExecutionMode exm)
{
    int left_x = log->position.x;
    int left_y = log->position.y;
    int length = log->length;

    int *corner = get_bottom_right_corner(log);
    int right_x = corner[0];
    int right_y = corner[1];

    if (left_x < left_bound)
    {
        log->position.x = left_bound;

        if (exm == PROCESS)
        {

        }
        else
        {

        }

        return true;
    }

    if (right_x > right_bound)
    {
        log->position.x = right_bound;

        if (exm == PROCESS)
        {

        }
        else
        {

        }

        return true;
    }

    free(corner);

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
    int x = vehicle->position.x;
    int y = vehicle->position.y;
    int length = vehicle->length;

    int *corner = get_bottom_right_corner(vehicle);
    int corner_x = corner[0];
    int corner_y = corner[1];

    if (vehicle->action == LEFT && corner_x < left_bound)
    {
        vehicle->position.x = right_bound; // todo replace?
        return true;
    }

    if (vehicle->action == RIGHT && x > right_bound)
    {
        vehicle->position.x = left_bound; // todo replace?
        return true;
    }

    free(corner);

    return false;
}

/**
 * Calcola le coordinate dell'angolo in basso a destra dell'entita'.
 * @param entity L'entita'.
 * @return Le coordinate [x, y].
 */
int *get_bottom_right_corner(Entity *entity)
{
    int *corner = MALLOC(int, 2);

    corner[0] = entity->position.x + entity->length;
    corner[1] = entity->position.y - 1;

    return corner;
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
}