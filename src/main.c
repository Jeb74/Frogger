
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
    /*
    bool started = false;
    int choice = display(&started);
    started = true;
    if (choice == 0) printf("You choosed threads!\n");
    if (choice == 1) printf("You choosed processes!\n");
    if (choice == 2) printf("You choosed to quit!\n");
    sleep(10);
    endwin();
    */
    Board board = {
        .time_left = 300,
        .is_game_won = false,
        .lives_left = 3,
        .points = 0,
    };
    init_graphics(&board);
    LOWCOST_INFO choice = display(board.MAX_X, board.MAX_Y);
    if (choice == 2) {
        endwin();
        return 0;
    }
    erase();
    display_board(&board);
    getch();
    endwin();

    return 0;
}