
#include "headers/libraries.h"

/**
 * Inizializza la grafica.
 * @param board La tabella di gioco.
 */
void init_graphics(Board *board)
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
    curs_set(false);
    getmaxyx(stdscr, board->MAX_Y, board->MAX_X);
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    //init_graphics(&board);

    Board board = {
        .time_left = 300,
        .is_game_won = false,
        .lives_left = 3,
        .points = 0,
    };

    pthread_t clock_thread;
    ThreadArgs clock_args = {
        .board = &board,
    };
    create_thread(&clock_thread, manage_clock, &clock_args);

    SLEEP_SECONDS(100);
    cancel_thread(&clock_thread);
    SLEEP_SECONDS(100);
    /*
    LOWCOST_INFO choice = initial_menu(board.MAX_X, board.MAX_Y);
    if (choice == 2)
    {
        endwin();
        return 0;
    }

    erase();
    display_board(&board);
    getch();
    endwin(); 

    return 0; */
}