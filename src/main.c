#include "headers/structures.h"

void m_testing(int argc, char *argv[])
{
    Screen screen;

    init_graphics(&screen);
    LOWCOST_INFO result = -1;
    do
    {
        if (argc > 1)
            result = get_action(argc, argv);
        else
            result = initial_menu(screen.x, screen.y);

        if (result >= 3)
        {
            screen.exm = result % 3;
            result = 2;
        }
        else
            screen.exm = result;

        set_exm(screen.exm);

        if (screen.exm == 0)
            thread_mode_exec(screen);
        else if (screen.exm == 1) {
            process_mode_exec(screen);
        }

    } while (result != 2);
}

void a_testing()
{
    ;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    m_testing(argc, argv);
    //printf("%s\n", num_to_string(0, 3));
    return 0;
}