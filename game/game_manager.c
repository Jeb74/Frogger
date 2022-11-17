#include "game_manager.h"

void move(entity *_entity, enum coordinate _coordinate, int value)
{

}

void init_game()
{
    board _board = { NULL };

    MULTILEVEL_INIT_DEFAULT(int, map, BOARD_HEIGHT, BOARD_WIDTH, 0)

    _board.map = map;

    PRINT_MATRIX(_board.map, BOARD_HEIGHT, BOARD_WIDTH, " %d ");
}