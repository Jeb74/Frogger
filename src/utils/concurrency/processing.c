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
    HANDLE_ERROR(read(_pipe.accesses[READ], buff, size));
}

/**
 * Scrive sulla pipe.
 * @param buff  Il buffer.
 * @param _pipe La pipe.
 * @param size  La dimensione del contenuto del buffer.
 */
void writeto(void *buff, pipe_t _pipe, size_t size)
{
    HANDLE_ERROR(write(_pipe.accesses[WRITE], buff, size));
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

    bool result = select(FD_SETSIZE, &set, NULL, NULL, ACCEPTABLE_WAITING_TIME);

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

    bool result = select(FD_SETSIZE, NULL, &set, NULL, ACCEPTABLE_WAITING_TIME);

    /* Controlla se non sono presenti elementi nella pipe. */
    if (result)
    {
        writeto(buff, _pipe, size);
    }

    return result;
}

/**
 * Gestisce il tempo.
 * @param r Il pipe di lettura.
 * @param w Il pipe di scrittura.
*/
void time_process(pipe_t r, pipe_t w) 
{
    CLOSE_READ(w);
    CLOSE_WRITE(r);

    unsigned int start;
    LOWCOST_INFO info = 0;

    readfrm(&start, r, sizeof(unsigned int));
    time_t starting_time = time(NULL);

    while (start != 0) 
    {
        time_t current_time = time(NULL);

        if (readifready(&info, r, sizeof(LOWCOST_INFO)))
        {
            if (info) 
            {
                exit(EXIT_SUCCESS);
            }
        }

        if (starting_time < current_time && current_time - starting_time == 1) 
        {
            starting_time = current_time;
            start--;
            writeto(&start, w, sizeof(unsigned int));
        }
    }

    exit(EXIT_SUCCESS);
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
    pipe_t *arr = create_pipes(2, "writetime", "readtime");

    pid_t ptime = fork();

    if (ptime == -1) 
    {
        return 2;
    }
    else if (ptime == 0) 
    {
        time_process(findpn(2, arr, "writetime"), findpn(2, arr, "readtime"));
    }
    else 
    {
        // Logic
        pipe_t wt = findpn(2, arr, "writetime");
        pipe_t rt = findpn(2, arr, "readtime");
        CLOSE_READ(wt);
        CLOSE_WRITE(rt);

        writeto(&(board.max_time), wt, sizeof(unsigned int));

        // Game
        Bar lf = create_life_bar(&board);
        Bar tm = create_time_bar(&board);

        while(1)
        {
            fetch_time(rt, &(board.time_left));
            calculate_life_bar(&lf, &board);
            calculate_time_bar(&tm, &board);
            update_graphics(&board, lf, tm);
            
            if (board.time_left == 0) break;
        }

        return 2;
    }
}



