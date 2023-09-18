#include "headers/structures.h"

void func(void *args)
{
    GameArgs *game;
    unpack(args, game, THREAD, GAMEPKG);
    printf("%i\n", (*game).board->lifes_left);
}

void m_testing(int argc, char *argv[])
{
    Screen screen;
    
    init_graphics(&screen);
    LOWCOST_INFO result = -1;
    do
    {
        if (argc > 1)
            result = getAction(argc, argv);
        else
            result = initial_menu(screen.x, screen.y);
        
        if (result >= 3)
        {
            screen.exm = result % 3;
            result = 2;
        }
        else
            screen.exm = result;
        
        if (screen.exm == 0)
            thread_mode_exec(screen);
        else if (screen.exm == 1)
            process_mode_exec(screen);
        
    } while (result != 2);
}

void a_testing()
{
    Board board = {
        .max_time = 300,
        .time_left = 0,
        .is_game_won = false,
        .lifes_on_start = 43,
        .lifes_left = 43,
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
    create_thread(&clock_thread, manage_clock, pkg);

    SLEEP_SECONDS(2);

    /* test format_number */
    for (size_t d = 0; d < 10; d++)
    {
        char **scores = format_number(d, ' ', '@');

        for (size_t i = 0; i < SCORE_HEIGHT; i++)
        {
            printf("%s\n", scores[i]);
        }

        FREE_ALL(scores, SCORE_HEIGHT);

        printf("\n");
    }

    /* test format_clock_numeric */
    char *clock = format_clock_numeric(&board);
    printf("%s\n", clock);
    free(clock);

    /* test format_clock_bar */
    clock = format_clock_bar(&board);
    printf("%s\n", clock);

    free(clock);

    //getch();
    //endwin();
    free(pkg);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    m_testing(argc, argv);
    return 0;
}