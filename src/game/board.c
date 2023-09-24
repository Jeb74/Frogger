#include "../headers/board.h"

void init_worker(struct worker_args *worker_args)
{
    sem_init(worker_args->job_queue_count, 0, 0);
}

void enqueue_movement(struct worker_args *worker_args, void *raw_packet)
{
    // struct job *new_job = MALLOC(struct job, 1);

    // new_job->id = gen_num(0, INT32_MAX);
    // new_job->raw_packet = raw_packet;

    // struct job **job_queue = worker_args->job_queue;
    // pthread_mutex_t *job_queue_mutex = worker_args->job_queue_mutex;
    // sem_t *job_queue_count = worker_args->job_queue_count;

    // printf("[ENQ] MovePacket ID: %zu\n", new_job->id);

    // AQUIRE_LOCK(*job_queue_mutex);

    // new_job->next = *job_queue;
    // *job_queue = new_job;

    // sem_post(job_queue_count);

    // RELEASE_LOCK(*job_queue_mutex);
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

Entity *create_entity(LOWCOST_INFO id, EntityTypes type, Action action, Position p) 
{
    Entity *e = CALLOC(Entity, 1);
    e->action = action;
    e->id = id;
    e->type = type;
    e->position = p;

    switch(e->type) 
    {
        case TRUCK:
            e->length = gen_num(TRUCK_MIN_SIZE, TRUCK_MAX_SIZE);
            break;
        case ENEMY_BIRD:
        case ENEMY_FROG:
            e->length = FROG_WIDTH;
            break;
        case ENEMY_SNAKE:
        case CAR:
        case LOG:
            e->length = gen_num(LOG_MIN_WIDTH, LOG_MAX_WIDTH);
            break;
    }

    return e;
}

void add_to_queue(EntityQueue **eq, EntityQueue **eqm) 
{
    EntityQueue *_eqm = *eqm;
    EntityQueue *_eq = *eq;

    while(_eq->next != NULL) _eq = _eq->next;

    _eq->next = _eqm;
    // _eqm->id = _eq->id + 1;
    _eqm->e->id = _eqm->id;
}

Entity *walk_through(EntityQueue *eq, unsigned int indx) 
{
    EntityQueue *_eq = eq;

    while(_eq->id != indx) {
        if (_eq->next != NULL) break;
        _eq = _eq->next;
    }

    return _eq->e;
}

/**
 * 0 - 9
 * Crea una queue di entità predefinite per il gioco (TRONCHI, MACCHINE E CAMION).
 * @param board La board di gioco.
 * @return      La queue di entità.
*/
EntityQueue *create_queue(Board board) {
    Action action = gen_num(LEFT, RIGHT);
    EntityQueue *eq = NULL;
    for (int i = 0; i < STD_ENTITIES; i++) {
        action = i != 0 && i % 2 == 0 ? !action : action;

        EntityQueue *eqm = CALLOC(EntityQueue, 1);
        eqm->id = i;

        Position p;
        p.y = board.fp.y - ((i < 10) ? 2 + ((int) i / 2) * 2 : i * 2 - 6);
        p.x = action == LEFT ? board.screen_x : -1;

        eqm->e = create_entity(i, i < 10 ? gen_num(CAR, TRUCK) : LOG, action, p);
        eqm->next = NULL;
        
        if (eq == NULL) eq = eqm;
        else add_to_queue(&eq, &eqm);
    }
    return eq;
}


void *manage_entity_movement(void *args) {
    ExecutionMode exm = get_exm();
    bool process_mode = exm == PROCESS;
    EntityMovePacket *data;
    unpack(args, data, exm, ENTITY_PKG);

    LOWCOST_INFO signal;

    if (process_mode) {
        data->default_action = CALLOC(Action, 1);
        readfrm(data->default_action, data->sub_packet.carriage.p.c, sizeof(Action));
        CLOSE_READ(data->sub_packet.carriage.p.c);
        CLOSE_READ(data->sub_packet.carriage.p.s);
        CLOSE_WRITE(data->sub_packet.carriage.p.se);
    }

    while(!data->sub_packet.cancelled) 
    {
        SLEEP_MILLIS(100);

        if (process_mode) 
        {
            signal = !data->sub_packet.cancelled;
            readifready(&signal, data->sub_packet.carriage.p.se, sizeof(LOWCOST_INFO));

            if (signal == KILL_SIGNAL)
            {
                data->sub_packet.cancelled = true;
            }
            else if (signal == PAUSE_SIGNAL)
            {
                readfrm(&signal, data->sub_packet.carriage.p.se, sizeof(LOWCOST_INFO));
            }
            else {
                writeto(data->default_action, data->sub_packet.carriage.p.c, sizeof(Action));
                writeto(&signal, data->sub_packet.carriage.p.s, sizeof(bool));
            }
        }
        else
        {
            block(false);
            hopper(false);
            struct ActionData adata = {
                .action = *(data->default_action),
                .id = data->sub_packet.id
            };

            EXEC_WHILE_LOCKED(data->sub_packet.carriage.t.entity_mutex,
                              {
                                int *count = data->sub_packet.carriage.t.counter;

                                data->sub_packet.carriage.t.wbuffer[*count] = adata;
                                (*count)++;
                              })
        }
    }

    if (process_mode)
    {
        CLOSE_WRITE(data->sub_packet.carriage.p.s);
        CLOSE_WRITE(data->sub_packet.carriage.p.c);
        CLOSE_READ(data->sub_packet.carriage.p.se);
    }

    return;
}
