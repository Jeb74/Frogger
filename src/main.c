
#include "headers/libraries.h"

int main(int argc, char **argv) 
{
    srand(time(NULL));

    Board board = {
        .MAX_X = 20,
        .MAX_Y = 20,
        .is_game_won = false,
        .points = 0,
        .lives_left = 3,
        .time_left = 600
    };

    Entity entity = {
        .type = FROG,
        .symbol = get_entity_symbol(FROG),
        .position = { .x = 0, .y = 0 },
        .direction = NONE,
        .velocity = { .x = 0, .y = 0 },
        .size = { .x = 1, .y = 1 }
    };

    pthread_t timer_thread;
    ThreadArgs timer_args = { .board = &board };

    create_thread(&timer_thread, manage_clock, &timer_args);

    SLEEP_SECONDS(5);

    cancel_threads(&timer_thread, 1);

    SLEEP_SECONDS(5);

    return 0;
}