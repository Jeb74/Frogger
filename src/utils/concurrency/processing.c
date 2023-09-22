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

/**
 * Quando disponibile, prende il movimento della rana.
 * @param r Il pipe di lettura.
 * @param fp La posizione della rana.
 */
bool fetch_frog(pipe_t r, Position *fp) 
{
    Action action = NONE;
    bool shooting = false;

    if (readifready(&action, r, sizeof(Action))) 
    {
        fp->x += action == RIGHT ? 1 : action == LEFT ? -1 : 0;
        fp->y += action == UP ? -2 : action == DOWN ? 2 : 0;
        shooting = action == SHOOT;
    }

    return shooting;
}

void generate_entity(char name[PIPE_NAME]) {

}

void generate_entities(unsigned int en, Process **e, pipe_t **p) {

}

/**
 * Esecutore modalità processi.
 * @param screen    Lo screen.
 * @return          Il codice di uscita.
 */
LOWCOST_INFO process_mode_exec(Screen screen) 
{
    erase();
    refresh();
    Board board;
    INIT_COLORS;
    INIT_BOARD(board, screen);

    ExecutionMode exm = get_exm();

    // Processing for real
    pipe_t *arr = create_pipes(PAS, "writetime", "readtime", "writeaction", "readaction", "readysignal");

    pipe_t wt = findpn(PAS, arr, "writetime");
    pipe_t rt = findpn(PAS, arr, "readtime");
    pipe_t wa = findpn(PAS, arr, "writeaction");
    pipe_t ra = findpn(PAS, arr, "readaction");
    pipe_t rs = findpn(PAS, arr, "readysignal");

    Process time = palloc("time", manage_clock, pack(exm, CLOCK_PKG, rt, wt, rs, &(board.time_left)));
    if (time.status < 0) return 2;
    Process frog = palloc("frog", manage_frog, pack(exm, FROG_PKG, ra, wa, rs, 127));
    if (frog.status < 0) return 2;

    CLOSE_READ(wt);
    CLOSE_WRITE(rt);
    CLOSE_READ(wa);
    CLOSE_WRITE(ra);
    CLOSE_WRITE(rs);

    writeto(&(board.max_time), wt, sizeof(unsigned int));

    // Process *entities;
    // pipe_t *pipes;
    // generate_entities(STD_ENTITIES, &entities, &pipes);

    bool signal = false;
    do
    {
        //EntityQueue queue = fetch_entities(&pipes);
        fetch_frog(ra, &(board.fp));
        fetch_time(rt, &(board.time_left));
        update_graphics(&board/*, queue */);
        readfrm(&signal, rs, sizeof(bool));
    } while (signal);

    return 2;
}



