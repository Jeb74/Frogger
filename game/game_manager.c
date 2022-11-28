#include "game_manager.h"

void move_inert(board *b)
{
    for (short i = 0; i < R_BOARD_HEIGHT; i++) {

        enum row_types curr_t = b->specifics[i].type;

        if (curr_t == WALKABLE || curr_t == LAWN || curr_t == DEN) {
            continue;
        }

        enum directions *curr = &(b->specifics[i].direction);
        bool is_right = *curr == right;

        for (short j = is_right ? 0 : R_BOARD_WIDTH - 1, once = 0; is_right ? j < R_BOARD_WIDTH : j >= 0; is_right ? j++ : j--) {

            cell *tmp = GET(b->cells, i, j);
            entity *_entity = tmp->_entity;

            if ((is_right && j == R_BOARD_WIDTH - 1) || (!is_right && j == 0)) {
                if (_entity) {
                    if (_entity->curr_width == _entity->width) {
                        tmp->_entity = NULL;
                    }
                    else {
                        _entity->curr_width++;
                    }
                }
            }
            else {
                tmp->_entity = b->cells[i][j + (is_right ? 1 : -1)]._entity;
            }

            if (curr_t == RIVER) {

                entity *far_right = b->cells[i][0]._entity;
                entity *far_left = b->cells[i][R_BOARD_WIDTH - 1]._entity;

                if ((is_right ? far_right : far_left) && !once) {
                    *curr = is_right ? left : right;
                    once++;
                }
            }
        }
    }
}

void move(void *element, enum directions direction)
{

}

enum row_types setType(unsigned i)
{
    if (i == 0) return WALKABLE;
    if (i <= HIGHWAY) return HIGHWAY;
    if (i == LAWN) return LAWN;
    if (i <= RIVER) return RIVER;
    return DEN;
}

/**
 * Slitta le entita' della riga nella direzione desiderata.
 * @param _board    La tabella di gioco.
 */
void spin_row(board *_board)
{

}

board genMap()
{
    board _board;

    ALLOC(_board.cells, cell *, R_BOARD_HEIGHT, CALLOC) // 13

    for (short i = 0; i < R_BOARD_HEIGHT; i++) {
        ALLOC(_board.cells[i], cell, R_BOARD_WIDTH, CALLOC) // 15
        _board.specifics[i].direction = (i == 0) ? rand()%SAFEDIM : !_board.specifics[i-1].direction;
        _board.specifics[i].type = setType(i);
        for (short j = 0; j < R_BOARD_WIDTH; j++) {
            _board.cells[i][j]._entity = NULL;
            _board.cells[i][j].proj = NULL;
        }
    }

    return _board;
}

/**
 * Controlla il numero di entità nella riga.
 * @param b La tabella di gioco.
 * @return Se la riga e' vuota (true) o meno (false).
 */
short get_entity_number(board *b, unsigned short line)
{
    short count = 0;
    long long int entity_addr = 0;
    enum directions dir = b->specifics[line].direction;

    bool is_right = dir == right;

    for (short i = is_right ? R_BOARD_WIDTH - 1 : 0; is_right ? i >= 0 : i < R_BOARD_WIDTH; is_right ? i-- : i++)
    {
        entity *en = b->cells[line][i]._entity;

        if (en) {
            long long int addr = GET_ADDR(en);

            if (addr != entity_addr) {
                entity_addr = addr;
                count++;
            }
        }
    }
    return count;
}

static void print_board(board *b)
{
    printf(" ");
    for (int i = 0; i <= R_BOARD_HEIGHT; ++i)
    {
        printf(" %d ", i);
    }
    printf("\n");

    for (short i = R_BOARD_HEIGHT - 1; i >= 0; i--)
    {
        printf("|");
        for (short j = R_BOARD_WIDTH - 1; j >= 0; j--)
        {
            if (b->cells[i][j]._entity != NULL) {
                //printf(" %c ", get_type(b->cells[i][j]._entity));GRE
                coloredText("   ", RESET, get_type(b->cells[i][j]._entity));
            }
            else {
                if (i == LAWN || i == WALKABLE || i == DEN) {
                    coloredText("   ", RESET, WHITE + COLOR_INTENSITY);
                }
                else {
                    if (i >= LAWN && i <= RIVER) {
                        coloredText("   ", RESET, BLUE);
                    }
                    else {
                        coloredText("   ", RESET, BLACK + COLOR_INTENSITY);
                    }
                }
            }
        }
        printf("\n");
    }
    printf("\n\n\n");
}

static bool add_entity(board *b)
{
    entity *e;
    ALLOC(e, entity, 1, CALLOC)
    e->type = rand() % 3 + 1;
    e->width = WIDTHOF(e->type);
    ALLOC(e->attributes, union attr, (e->type == LOG) ? e->width : 5, CALLOC)

    enum row_types this_t   = ZONEOF(e->type);
    short line              = rand() % R_RIVER_LANES + ((this_t == RIVER) ? LAWN + 1 : R_FROG_HEIGHT);
    bool rowIsFree          = false;

    short eNum = get_entity_number(b, line);
    if ((eNum < 3 && e->type != LOG) || (eNum < 1 && e->type == LOG)) {

        cell *current   = b->cells[line];
        bool isRight    = b->specifics[line].direction == right;

        if (isRight)
        {
            rowIsFree = !(current[R_BOARD_WIDTH - 1]._entity || current[R_BOARD_WIDTH - (SAFEDIM + 1)]._entity);

            if (rowIsFree) {
                current[R_BOARD_WIDTH - 1]._entity = &(*e);
            }
        }
        else
        {
            rowIsFree = !(current[0]._entity || current[SAFEDIM]._entity);

            if (rowIsFree) {
                current[0]._entity = &(*e);
            }
        }

        e->curr_width = rowIsFree ? 1 : 0;
    }

    return rowIsFree;
}

void init_game()
{
    board _board = gen_map();
    print_board(&_board);
    bool val = add_entity(&_board);
    print_board(&_board);

    for (int i = 0; i < 300; i ++) {
        move_inert(&_board);
        val = add_entity(&_board);
        //printf("\n\n\n%i\n\n\n", val);
        print_board(&_board);
        usleep(500000);
    }
}