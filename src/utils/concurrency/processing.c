#include "../../headers/processing.h"

/**
 * Trova una pipe sulla base del nome.
 * @param len   La lunghezza dell'array.
 * @param pipes L'array delle pipe.
 * @param name  Il nome della pipe.
*/
pipe_t findpn(int len, pipe_t pipes[], char name[PIPE_NAME])
{
    for (int i = 0; i < len; i++) 
    {
        if (strcmp(name, pipes[i].name) == 0) 
        {
            return pipes[i];
        }
    }

    return FAILED_PIPE;
}

/**
 * Crea una pipe.
 * @param name  Il nome della pipe.
*/
pipe_t create_pipe(char name[PIPE_NAME]) 
{
    pipe_t p;

    p.name = CALLOC(char, PIPE_NAME);
    strcpy(p.name, name);

    HANDLE_ERROR(pipe(p.accesses) + 1);

    int len = strlen(name);

    p.name = REALLOC(char, p.name, len + TERM);
    p.name[len] = '\0';

    return p;
}

/**
 * Crea multiple pipe.
 * @param size  La dimensione dell'array.
 * @param ...   I nomi delle pipe.
*/
pipe_t *create_pipes(int size, ...) 
{
    pipe_t *array = CALLOC(pipe_t, size);

    va_list nl;
    va_start(nl, size);
    
    for (int i = 0; i < size; i++) 
    {
        array[i] = create_pipe(va_arg(nl, char*));
    }

    va_end(nl);

    return array;
}

/**
 * Legge dalla pipe.
 * @param buff  Il buffer.
 * @param _pipe La pipe.
 * @param size  La dimensione del contenuto del buffer.
*/
void readfrm(void *buff, pipe_t _pipe, size_t size) 
{
    //printf("----- %s\n", _pipe.name);
    HANDLE_ERROR(read(_pipe.accesses[READ], buff, size)+1);
}

/**
 * Scrive sulla pipe.
 * @param buff  Il buffer.
 * @param _pipe La pipe.
 * @param size  La dimensione del contenuto del buffer.
 */
void writeto(void *buff, pipe_t _pipe, size_t size)
{
    HANDLE_ERROR(write(_pipe.accesses[WRITE], buff, size)+1);
}

/**
 * Legge dalla pipe se è pronta.
 * @param buff  Il buffer.
 * @param _pipe La pipe.
 * @param size  La dimensione del contenuto del buffer.
*/
bool readifready(void *buff, pipe_t _pipe, size_t size) 
{
    fd_set set;

    /* Set di file descriptors. */
    FD_ZERO(&set);
    FD_SET(_pipe.accesses[READ], &set);

    bool result = select(FD_SETSIZE, &set, NULL, NULL, ACCEPTABLE_WAITING_TIME) > 0;

    /* Controlla se sono presenti elementi nella pipe. */
    if (result)
    {
        readfrm(buff, _pipe, size);
    }

    return result;
}

/**
 * Scrive sulla pipe se è pronta.
 * @param buff  Il buffer.
 * @param _pipe La pipe.
 * @param size  La dimensione del contenuto del buffer.
 */
bool writeifready(void *buff, pipe_t _pipe, size_t size) 
{
    fd_set set;

    /* Set di file descriptors. */
    FD_ZERO(&set);
    FD_SET(_pipe.accesses[WRITE], &set);

    /* Controlla se non sono presenti elementi nella pipe. */
    bool result = select(FD_SETSIZE, NULL, &set, NULL, ACCEPTABLE_WAITING_TIME) > 0;

    if (result)
    {
        writeto(buff, _pipe, size);
    }

    return result;
}

/**
 * Crea un nuovo processo.
 * @param pname Il nome del processo.
 * @param _func Il puntatore alla funzione.
 * @param pkg   Il pacchetto.
*/
Process palloc(char pname[PIPE_NAME], void (*_func)(void*), Package *pkg)
{
    Process p = {.status=0};

    p.name = CALLOC_TERM(char, strlen(pname));
    strcpy(p.name, pname);

    p.pid = fork();
    
    if (p.pid == 0)
    {
        _func(pkg);
        exit(EXIT_SUCCESS);
    }
    else if (p.pid == -1) 
    {
        p.status = -1;
    }
    
    return p;
}

/*
TODO: implementare nella funzione display_board un sistema di riconoscimento del tipo
dell'entità per poter scegliere il colore più appropriato/immagine.
implementare una funzione di creazione e rimozione delle entità.
modificare l'else della process_mode_exec per ottimizzare le chiamate ai processi figli.
*/

/**
 * Quando disponibile, prende il valore del tempo.
 * @param r Il pipe di lettura.
 * @param time Il tempo.
 */
void fetch_time(pipe_t r, unsigned int *time)
{
    readifready(time, r, sizeof(unsigned int));
}

void send_pause_menu() 
{
    wgetch(stdscr);
    return;
}

/**
 * Quando disponibile, prende il movimento della rana.
 * @param r Il pipe di lettura.
 * @param fp La posizione della rana.
 */
LOWCOST_INFO fetch_frog(pipe_t r, Position *fp, Pipes services, LOWCOST_INFO *service, WINDOW *w) 
{
    Action action = NONE;
    bool shooting = false;

    if (readifready(&action, r, sizeof(Action)) && action != RQPAUSE && action != RQQUIT) 
    {
        fp->x += action == RIGHT ? 1 : action == LEFT ? -1 : 0;
        fp->y += action == UP ? -2 : action == DOWN ? 2 : 0;
        shooting = action == SHOOT;
    }
    else if (action == RQPAUSE) {
        for (int i = 0; i < services.size; i++) writeto(service, services.pipes[i], sizeof(LOWCOST_INFO));
        show_pause_menu(w);
                     *service = RESUME_SIGNAL;
        for (int i = 0; i < services.size; i++) writeto(service, services.pipes[i], sizeof(LOWCOST_INFO));
    }
    else if (action == RQQUIT) {
        erase();
        return KILL_SIGNAL;
    }

    return shooting;
}

void generate_entity(unsigned int indx, pipe_t *c, pipe_t *se, pipe_t s, Process *p) {
    char *id = num_to_string(indx, -1);
    char *name = build_string("e_%s", id);
    free(id);
    char *wpname = build_string("%s_%s", "comm", name);
    char *rpname = build_string("%s_%s", "service", name);
    pipe_t *pipes = create_pipes(MIN_PAS, wpname, rpname);
    *c = pipes[0];
    *se = pipes[1];
    free(pipes);
    free(wpname);
    free(rpname);
    *p = palloc(name, manage_entity_movement, pack(PROCESS, ENTITY_PKG, *c, *se, s, indx, NONE));
    free(name);
}

Process *generate_entities(pipe_t **p, pipe_t s) {
    *p = CALLOC(pipe_t, STD_ENTITIES*2);
    Process *pr = CALLOC(Process, STD_ENTITIES);
    for (int i = 0, j = 0; i < STD_ENTITIES; ++i, j = i * 2) {
        generate_entity(i, &((*p)[j]), &((*p)[j+1]), s, &(pr[i]));
        if (pr[i].status < 0) {
            char *err = build_string("[Process Creation] %s has not completed its lifetime.\n", pr[i].name);
            perror(err);
            exit(EXIT_FAILURE);
        }
    }
    return pr;
}

void instruct_entities(EntityQueue *queue, Pipes *pipes, Pipes *services) {
    int services_size = (*services).size + (*pipes).size/MIN_PAS;
    Pipes new_pipes = {.size = STD_ENTITIES, .pipes = CALLOC(pipe_t, STD_ENTITIES)};
    services->size = services_size;
    (*services).pipes = REALLOC(pipe_t, (*services).pipes, services_size);
    for (int i = 0, s = MIN_PAS, n = 0; i < (*pipes).size; ++i) {
        pipe_t p = (*pipes).pipes[i];
        if (startswith(p.name, "service")) {
            (*services).pipes[s] = p;
            s++;
        }
        else {
            new_pipes.pipes[n] = p;
            Entity *e = walk_through(queue, n);
            writeto(&(e->action), p, sizeof(Action));
            n++;
        }
    }
    free((*pipes).pipes);
    *pipes = new_pipes;
}

ShootingRequest *pfetch_entities(Pipes comms, EntityQueue *eq) {
    Action action;
    ShootingRequest *rqs;
    int rqs_counter = 0;
    if (comms.size > STD_ENTITIES) rqs = CALLOC(ShootingRequest, comms.size - STD_ENTITIES);
    else rqs = NULL;
    for (int i = 0; i < comms.size; i++) {
        if (readifready(&action, comms.pipes[i], sizeof(Action))) {
            Entity *e = walk_through(eq, i);
            switch (action) {
                case LEFT:
                    e->position.x -= 1;
                    break;
                case RIGHT:
                    e->position.x += 1;
                    break;
                case UP:
                    e->position.y -= 1;
                    break;
                case DOWN:
                    e->position.y += 1;
                    break;
                case SHOOT:
                    rqs[rqs_counter] = (ShootingRequest) {.id=i, .proj_action=DOWN};
                    rqs_counter++;
                    break;
            }
        }
    }
    return rqs;
}

/**
 * Esecutore modalità processi.
 * @param screen    Lo screen.
 * @return          Il codice di uscita.
 */
void process_mode_exec(Screen screen, WINDOW **ws) 
{
    erase();
    refresh();
    Board board;
    INIT_COLORS;
    INIT_BOARD(board, screen);
    create_hideouts(screen.x, &board);
    bool signal = false;
    LOWCOST_INFO service = PAUSE_SIGNAL;

    ExecutionMode exm = get_exm();

    // Processing for real
    pipe_t *arr = create_pipes(PAS, TIME_COMMS, SERVICE_TIME, USER_COMMS, SERVICE_USER, READY_PIPE);

    pipe_t rt = findpn(PAS, arr, TIME_COMMS);
    pipe_t ra = findpn(PAS, arr, USER_COMMS);
    pipe_t rs = findpn(PAS, arr, READY_PIPE);

    Pipes services = {.pipes = CALLOC(pipe_t, MIN_PAS), .size = MIN_PAS};
    services.pipes[0] = findpn(PAS, arr, SERVICE_TIME);
    services.pipes[1] = findpn(PAS, arr, SERVICE_USER);

    free(arr);

    Process time = palloc("time", manage_clock, pack(exm, CLOCK_PKG, rt, services.pipes[0], rs, &(board.time_left)));
    if (time.status < 0) return 2;
    Process frog = palloc("frog", manage_frog, pack(exm, FROG_PKG, ra, services.pipes[1], rs, 127));
    if (frog.status < 0) return 2;

    writeto(&(board.max_time), rt, sizeof(unsigned int));

    Pipes pipes = {.size=STD_ENTITIES*2};
    int currently_created = STD_ENTITIES;
    Process *processes = generate_entities(&(pipes.pipes), rs);

    EntityQueue *queue = create_queue(board);
    instruct_entities(queue, &pipes, &services);

    CLOSE_WRITE(rt);
    CLOSE_WRITE(rs);
    CLOSE_WRITE(ra);
    ShootingRequest *rqs;
    unsigned int rqsize = 0;
    while (true) {   
        rqs = pfetch_entities(pipes, queue);
        LOWCOST_INFO sig = fetch_frog(ra, &(board.fp), services, &service, ws[GBOARD]);
        if (sig == KILL_SIGNAL) {
            destroy_entityqueue(queue);
            break;
        }
        else if (sig == true) {
            if (rqs == NULL) {
                rqsize = 1;
                rqs = CALLOC(ShootingRequest, rqsize);
            }
            else {
                rqsize = (pipes.size - STD_ENTITIES) + 1;
                rqs = REALLOC(ShootingRequest, rqs, rqsize);
            }
            rqs[rqsize - 1] = (ShootingRequest) {.id=FROG_ID, .proj_action=UP};
        }
        service = PAUSE_SIGNAL;
        fetch_time(rt, &(board.time_left));
        update_graphics(&board, NULL, ws);
        if (board.time_left <= 0) {
            board.is_game_won = false;
            break;
        }
        readfrm(&signal, rs, sizeof(bool));
    }
    service = KILL_SIGNAL;
    free(frog.name);
    free(time.name);
    for (int i = 1; i < services.size; i++) {
        writeto(&service, services.pipes[i], sizeof(LOWCOST_INFO));
        if (i == 1) waitpid(frog.pid, NULL, 0);
        else {
            waitpid(processes[i-2].pid, NULL, 0);
            free(processes[i-2].name);
        }
    }
    free(processes);
    erase();
    if (board.is_game_won) send_win_menu(ws[GBOARD]);
    else send_lose_menu(ws[GBOARD]);
    return;
}



