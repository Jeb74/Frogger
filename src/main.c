#include "headers/structures.h"

void func(void *args)
{
    GameArgs *game;
    unpack(args, game, THREAD, GAMEPKG);
    printf("%i\n", (*game).board->lives_left);
}

void m_testing()
{
    // LOWCOST_INFO choice = initial_menu(board.MAX_X, board.MAX_Y);
    // if (choice == 2)
    // {
    //     endwin();
    //     return 0;
    // }

    // erase();
    // display_board(&board);
    // getch();
    // endwin();
}

void a_testing()
{
    Board board = {
        .max_time = 300,
        .time_left = 0,
        .is_game_won = false,
        .lives_left = 3,
        .points = 0,
        // ho cambiato la struct board per renderla più coerente col suo impiego.
        };

    // init_graphics(&board);
    // display_board(&board);

    GameArgs game = {
        .board = &board,
        .entity = NULL};

    Package *pkg = pack(THREAD, GAMEPKG, &game);

    pthread_t clock_thread;
    //create_thread(&clock_thread, manage_clock, pkg);

    getch();
    SLEEP_SECONDS(100);
    endwin();
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    Screen screen;
    init_graphics(&screen);
    // a_testing();
    // m_testing();
    LOWCOST_INFO result = -1;
    do {
        if (argc > 1) result = getAction(argc, argv);
        else result = initial_menu(screen.x, screen.y);
        if (result >= 3) {
            screen.exm = result % 3;
            result = 2;
        } 
        else screen.exm = result;
        if (screen.exm == 0) thread_mode_exec(screen);
        else if (screen.exm == 1) process_mode_exec(screen);
    } while(result != 2);
    return 0;
}