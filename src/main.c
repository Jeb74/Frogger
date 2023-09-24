#include "headers/structures.h"

void m_testing(int argc, char *argv[])
{
    Screen screen;
    ExecutionMode exm;
    init_debugger("debug.txt");

    init_graphics(&screen);
    WINDOW **ws = create_windows(screen);
    LOWCOST_INFO result = -1;
    do
    {
        if (argc > 1)
            result = get_action(argc, argv);
        else
            result = initial_menu(screen.x, screen.y);

        if (result >= 3)
        {
            exm = result % 3;
            result = 2;
        }
        else
        {
            exm = result;
        }

        set_exm(exm);

        if (exm == THREAD)
            thread_mode_exec(screen, ws);
        else if (exm == PROCESS)
            process_mode_exec(screen, ws);

    } while (result != 2);

    endwin();
    close_debugger();
}

void a_testing()
{
    set_exm(THREAD);

    /*
     * Clock test.
     */
    {
        Board board = {.max_time = 5, .time_left = 5};

        pthread_mutex_t clockMutex = PTHREAD_MUTEX_INITIALIZER;

        Package *clockPkg = pack(get_exm(), CLOCK_PKG, &clockMutex, &(board.time_left), false);

        pthread_t clockThread;
        create_thread(&clockThread, manage_clock, clockPkg);

        join_thread(&clockThread);
        destroy_package(clockPkg);
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    m_testing(argc, argv);
    
    // a_testing();
    return 0;
}