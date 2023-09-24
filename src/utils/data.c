#include "../headers/data.h"

/**
 * Imposta la modalità di esecuzione.
 * @param exm   La modalità di esecuzione.
 */
void set_exm(ExecutionMode new_mode)
{
    exm = new_mode;
}

/**
 * Restituisce la modalità di esecuzione.
 * @return  La modalità di esecuzione.
 */
ExecutionMode get_exm()
{
    return exm;
}

void block(bool value)
{
    static bool once = false;
    static sem_t semaphore;
    static int sem_val;

    if (!once)
    {
        once = true;
        sem_init(&semaphore, 0, 1);
    }

    sem_getvalue(&semaphore, &sem_val);

    if (!sem_val && !value)
    {
        sem_post(&semaphore);
    }
    else if (value)
    {
        sem_wait(&semaphore);
    }
}

void hopper(bool value)
{
    static bool once = false;
    static sem_t semaphore;
    static int sem_val;

    if (!once)
    {
        once = true;
        sem_init(&semaphore, 0, 1);
    }

    sem_getvalue(&semaphore, &sem_val);

    if (!sem_val && !value)
    {
        sem_wait(&semaphore);
    }
    else if (value)
    {
        sem_wait(&semaphore);
        // send pause
        sem_post(&semaphore);
    }
}

/**
 * Inizializza il semaforo per il worker.
 * @param worker_args   I parametri del worker.
*/
void _init_worker(struct worker_args *worker_args)
{
    sem_init(worker_args->job_queue_count, 0, 0);
}

/**
 * Funziona eseguita dal thread worker.
 * @param arg   I parametri del worker.
 */
void _worker(void *arg)
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

    //     // TODO CARRY WORK OUT
    //     printf("Job ID: %zu\n", next_job->id);
    //     printf("Packet: %i\n", *((int *) next_job->raw_packet));

    //     free(next_job);
    // }
}

/**
 * Inserisce un job nella coda del worker.
 * @param worker_args   I parametri del worker.
 * @param raw_packet    Il pacchetto da inserire.
*/
void _enqueue_job(struct worker_args *worker_args, void *raw_packet)
{
    // struct job *new_job = MALLOC(struct job, 1);

    // new_job->id = gen_num(0, INT32_MAX);
    // new_job->raw_packet = raw_packet;

    // struct job **job_queue = worker_args->job_queue;
    // pthread_mutex_t *job_queue_mutex = worker_args->job_queue_mutex;
    // sem_t *job_queue_count = worker_args->job_queue_count;

    // printf("Enqueuing job with ID: %zu\n", new_job->id);

    // AQUIRE_LOCK(*job_queue_mutex);

    // new_job->next = *job_queue;
    // *job_queue = new_job;

    // sem_post(job_queue_count);

    // RELEASE_LOCK(*job_queue_mutex);
}