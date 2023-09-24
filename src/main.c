#include "headers/structures.h"


int main(int argc, char *argv[])
{
    srand(time(NULL));

    Screen screen;
    ExecutionMode exm;

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
    
    return 0;
}