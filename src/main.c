#include "headers/structures.h"

void m_testing(int argc, char *argv[])
{
    Screen screen;
    ExecutionMode exm;

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
            exm = result % 3;
            result = 2;
        }
        else
        {
            exm = result;
        }

        set_exm(exm);

        if (exm == 0)
            thread_mode_exec(screen);
        else if (exm == 1) {
            process_mode_exec(screen);
        }

    } while (result != 2);
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

        for (size_t i = 0; i < 5 * 10; i++)
        {
            printf("Board time: %i\n", board.time_left);
            SLEEP_MILLIS(100);
        }

        join_thread(&clockThread);
        printf("Board time: %i\n", board.time_left);

        destroy_package(clockPkg);
    }

    /*
     * Frog test.
     */
    // {
    //     Screen screen;
    //     init_graphics(&screen);

    //     Board board = {.max_time = 5, .time_left = 5};

    //     pthread_mutex_t frogMutex = PTHREAD_MUTEX_INITIALIZER;

    //     Position position = {.x = 5, .y = 5};

    //     Entity entity = { .type = FROG, .position = position};

    //     EntityMovePacket frogMovePacket =
    //     {
    //         .mutex = &frogMutex,
    //         .entity = &entity,
    //         .cancelled = false
    //     };

    //     Package *pkg = pack(get_exm(), GENPKG, &frogMovePacket);

    //     pthread_t frogThread;
    //     create_thread(&frogThread, &live_frog, pkg);

    //     pthread_join(frogThread, NULL);
    // }

    /*
     * God test.
     */
    // {

    //     pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
    //     sem_t job_queue_count;
    //     struct job *job_queue;

    //     struct worker_args worker_args =
    //     {
    //         .job_queue = &job_queue,
    //         .job_queue_mutex = &job_queue_mutex,
    //         .job_queue_count = &job_queue_count
    //     };

    //     init_worker(&worker_args);
        
    //     Package *pkg = pack(get_exm(), GENPKG, &worker_args);

    //     pthread_t testThread;
    //     create_thread(&testThread, &worker, pkg);

    //     for (size_t i = 0; i < 100; i++)
    //     {
    //         for (size_t i = 0; i < 10; i++)
    //         {
    //             int *a = MALLOC(int, 1);
    //             *a = i;
    //             enqueue_job(&worker_args, a);

    //             SLEEP_MILLIS(1);
    //         }

    //         SLEEP_MILLIS(100);
    //     }

    //     pthread_join(testThread, NULL);
    //     destroy_package(pkg);
    // }

    // {
    //     pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
    //     sem_t job_queue_count;
    //     struct job *job_queue;

    //     struct worker_args god_args =
    //     {
    //         .job_queue = &job_queue,
    //         .job_queue_mutex = &job_queue_mutex,
    //         .job_queue_count = &job_queue_count
    //     };

    //     init_worker(&god_args);

    //     pthread_t godThread;

    //     ThreadPacket threadPacket =
    //     {
    //         .thread = &godThread,
    //         .type = GOD,
    //         .cancelled = false
    //     };

    //     Package *threadPackage = pack(get_exm(), GENPKG, &threadPacket);
    //     Package *godPackage = pack(get_exm(), GENPKG, &god_args);

    //     struct worker_thread worker_thread =
    //     {
    //         .threadPackage = threadPackage,
    //         .workerPackage = godPackage
    //     };

    //     Package *godPackage = pack(get_exm(), GENPKG, &god_args);
    
    //     create_thread(&godThread, god, &worker_thread);
    //     join_thread(&godThread);
    // }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    m_testing(argc, argv);
    // a_testing();
    return 0;
}