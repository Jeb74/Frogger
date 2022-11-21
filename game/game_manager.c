#include "game_manager.h"

void move(board _board, bool direction)
{
    entity *first   = _board.cells[0][0]._entity;
    entity *last    = _board.cells[0][R_BOARD_WIDTH - 1]._entity;



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
    }

    return _board;
}

/**
 * Controlla se la riga e' vuota, potrebbe servire.
 * @param _board    La tabella di gioco.
 * @return          Se la riga e' vuota (true) o meno (false).
 */
bool row_is_empty(board *_board)
{
    for (int i = 0; i < R_BOARD_WIDTH; i++) {
        if (_board->cells[i]->_entity != NULL) {
            return false;
        }
    }
    return true;
}

bool entity_dies(entity *_entity)
{
    _entity->attributes.frog_status;
}

void init_game()
{
    board _board = genMap();
}