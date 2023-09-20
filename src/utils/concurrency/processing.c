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

    int len = strlen(p.name);

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

    bool result = select(FD_SETSIZE, NULL, &set, NULL, ACCEPTABLE_WAITING_TIME) > 0;

    /* Controlla se non sono presenti elementi nella pipe. */
    if (result)
    {
        writeto(buff, _pipe, size);
    }

    return result;
}

Process palloc(char pname[PIPE_NAME], void (*_func)(void*), Package *pkg) {
    Process p = {.status=0};
    p.name = CALLOC(char, strlen(pname)+1);
    strcpy(p.name, pname);
    p.pid = fork();
    if (p.pid == 0) {
        _func(pkg);
        exit(EXIT_SUCCESS);
    }

    if (p.pid == -1) p.status = -1;
    return p;
}

/*
TODO: implementare nella funzione display_board un sistema di riconoscimento del tipo
dell'entità per poter scegliere il colore più appropriato/immagine.
implementare una funzione di creazione e rimozione delle entità.
modificare l'else della process_mode_exec per ottimizzare le chiamate ai processi figli.
*/

/**
 * Gestisce il processo del tempo.
 * @param r Il pipe di lettura.
 * @param time Il tempo.
*/
void fetch_time(pipe_t r, unsigned int *time)
{
    readifready(time, r, sizeof(unsigned int));
}

bool fetch_frog(pipe_t r, Position *fp) {
    Action a = NONE;
    if (readifready(&a, r, sizeof(Action))) {
        bool shooting = false;
        switch(a) {
            case UP:
                fp->y -= 2;
                break;
            case DOWN:
                fp->y += 2;
                break;
            case RIGHT:
                fp->x += 1;
                break;
            case LEFT:
                fp->x -= 1;
                break;
            case SHOOT:
                shooting = true;
                break;
            case NONE:
                break;
        }
        return shooting;
    }
    return false;
}

/**
 * Esecutore modalità processi.
 * @param screen    Lo screen.
 * @return          Il codice di uscita.
*/
LOWCOST_INFO process_mode_exec(Screen screen) 
{
    erase();
    Board board;
    INIT_COLORS;
    INIT_BOARD(board, screen);
    endwin();

    // Processing for real
    pipe_t *arr = create_pipes(4, "writetime", "readtime", "writeaction", "readaction");

    Process time = palloc("time", manage_clock, pack(screen.exm, GENPKG, arr));
    if (time.status < 0) return 2;
    Process frog = palloc("frog", manage_frog, pack(screen.exm, GENPKG, arr));
    if (frog.status < 0) return 2;
    // Logic
    pipe_t wt = findpn(4, arr, "writetime");
    pipe_t rt = findpn(4, arr, "readtime");
    pipe_t wa = findpn(4, arr, "writeaction");
    pipe_t ra = findpn(4, arr, "readaction");
    CLOSE_READ(wt);
    CLOSE_WRITE(rt);
    CLOSE_READ(wa);
    CLOSE_WRITE(ra);

    writeto(&(board.max_time), wt, sizeof(unsigned int));

    // Game
    Bar lf = create_life_bar(&board);
    Bar tm = create_time_bar(&board);
    Bar sc = {.value = 707};

    int x = 0,y = 0;
    while(1)
    {
        fetch_frog(ra, &(board.fp));
        fetch_time(rt, &(board.time_left));
        sc.value = board.time_left;
        calculate_life_bar(&lf, &board);
        calculate_time_bar(&tm, &board);
        update_graphics(&board, lf, tm, sc);
        
        if (board.time_left == 0) break;
    }
    return 2;
}



