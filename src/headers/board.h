#ifndef FROGGER_BOARD_H
#define FROGGER_BOARD_H

#include "structures.h"

// Inizializza la board.
#define INIT_BOARD(board, screen)                                   \
    {                                                               \
        board.screen_x = screen.x;                                  \
        int screen_mid = ((int)screen.y / 2);                       \
        board.top_y = screen_mid - (screen.y % 2 == 0) ? 8 : 9;     \
        board.low_y = screen_mid + ((screen.y % 2 == 0) ? 16 : 15); \
        board.is_game_won = false;                                  \
        board.points = 0;                                           \
        board.lifes_on_start = 3;                                   \
        board.lifes_left = 3;                                       \
        board.max_time = 300;                                       \
        board.time_left = 300;                                      \
        board.fp.y = board.low_y - 1;                               \
        board.fp.x = (int)(board.screen_x / 2);                     \
    }

void *god(void *arg);
void init_worker(struct worker_args *worker_args);
void enqueue_movement(struct worker_args *worker_args, void *raw_packet);
EntityQueue *create_queue();
void *manage_entity_movement(void *args);
Entity walk_through(EntityQueue *eq, unsigned int indx);

#endif // FROGGER_BOARD_H