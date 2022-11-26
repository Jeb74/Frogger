#include "game_manager.h"

void move_inert(board _b)
{
    for (short i = 0; i < R_BOARD_HEIGHT; i++) {
        enum row_types curr_t = _b.specifics[i].type;
        if (curr_t != WALKABLE && curr_t != LAWN && curr_t != DEN) {
            enum directions curr = _b.specifics[i].direction;
            for (short j = (curr == right) ? 0 : R_BOARD_WIDTH - 1;
                 (curr == right) ? j < R_BOARD_WIDTH : j >= 0;
                 (curr == right) ? j++ : j--) {
                // Creo un puntatore per comodità
                cell *tmp = &(_b.cells[i][j]);

                if ((curr == right && j == R_BOARD_WIDTH - 1) || (curr == left && j == 0)) {
                    if (tmp->_entity) {
                        if (tmp->_entity->curr_width == tmp->_entity->width) {
                            tmp->_entity = NULL;
                        }
                        else {
                            tmp->_entity->curr_width++;
                        }
                    }
                }
                else {
                    tmp->_entity = _b.cells[i][j + ((curr == right) ? 1 : -1)]._entity;
                }
            }
        }
    }
}

void move(void *element, enum directions direction, enum move_e type)
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

void print_board(board b)
{
    for (short i = R_BOARD_HEIGHT - 1; i >= 0; i--) {
        for (short j = R_BOARD_WIDTH - 1; j >= 0; j--) {
            if (b.cells[i][j]._entity) printf("[E]");
            else printf("[ ]");
        }
        printf("\n");
    }
    printf("\n\n\n");
}

void init_game()
{
    board _board = genMap();
    print_board(_board);
    entity t = {
            .type = TRUCK,
            .width = 3,
            .curr_width = 1,
            };
    entity e = {
            .type = TRUCK,
            .width = 3,
            .curr_width = 1,
            };

    if (_board.specifics[1].direction == right) _board.cells[1][R_BOARD_WIDTH-1]._entity = &t;
    else _board.cells[1][0]._entity = &t;

    if (_board.specifics[2].direction == right) _board.cells[2][R_BOARD_WIDTH-1]._entity = &e;
    else _board.cells[2][0]._entity = &e;

    print_board(_board);
    for (int i = 0; i < 10; i ++) {
        move_inert(_board);
        print_board(_board);
    }
}

// FK362CM