#include "../headers/board.h"

void init_worker(struct worker_args *worker_args)
{
    sem_init(worker_args->job_queue_count, 0, 0);
}

void enqueue_movement(struct worker_args *worker_args, void *raw_packet)
{
    struct job *new_job = MALLOC(struct job, 1);

    new_job->id = gen_num(0, INT32_MAX);
    new_job->raw_packet = raw_packet;

    struct job **job_queue = worker_args->job_queue;
    pthread_mutex_t *job_queue_mutex = worker_args->job_queue_mutex;
    sem_t *job_queue_count = worker_args->job_queue_count;

    printf("[ENQ] MovePacket ID: %zu\n", new_job->id);

    AQUIRE_LOCK(*job_queue_mutex);

    new_job->next = *job_queue;
    *job_queue = new_job;

    sem_post(job_queue_count);

    RELEASE_LOCK(*job_queue_mutex);
}

void *god(void *arg)
{
    // ExecutionMode exm = get_exm();

    // void *data;
    // unpack(arg, data, exm, GENPKG);

    // struct worker_args *worker_args = (struct worker_args *)data;

    // struct job **job_queue = worker_args->job_queue;
    // pthread_mutex_t *job_queue_mutex = worker_args->job_queue_mutex;
    // sem_t *job_queue_count = worker_args->job_queue_count;

    // struct job *next_job;

    // while (true)
    // {
    //     sem_wait(job_queue_count);

    //     AQUIRE_LOCK(*job_queue_mutex);

    //     next_job = *job_queue;
    //     *job_queue = (*job_queue)->next;

    //     RELEASE_LOCK(*job_queue_mutex);
        
    //     // TODO CHECKS
    //     printf("[EXC] MovePacket ID: %zu\n", next_job->id);

    //     free(next_job);
    // }
}