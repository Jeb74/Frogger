#include "../../headers/processing.h"


pipe_t findpn(int len, pipe_t pipes[], char name[PIPE_NAME]) {
    for (int i = 0; i < len; i++) {
        if (strcmp(name, pipes[i].name) == 0) return pipes[i];
    }
    return FAILED_PIPE;
}

pipe_t create_pipe(char name[PIPE_NAME]) {
    pipe_t p;
    p.name = CALLOC(char, PIPE_NAME);
    strcpy(p.name, name);
    HANDLE_ERROR(pipe(p.accesses)+1);
    int len = strlen(p.name);
    p.name = REALLOC(char, p.name, len+1);
    p.name[len] = '\0';
    return p;
}

pipe_t *create_pipes(int size, ...) {
    va_list nl;
    va_start(nl, size);
    pipe_t *array = CALLOC(pipe_t, size);
    for (int i = 0; i < size; i++) array[i] = create_pipe(va_arg(nl, char*));
    va_end(nl);
    return array;
}

void readfrm(void *buff, pipe_t _pipe, size_t size) {
    HANDLE_ERROR(read(_pipe.accesses[READ], buff, size));
}

void writeto(void *buff, pipe_t _pipe, size_t size) {
    HANDLE_ERROR(write(_pipe.accesses[WRITE], buff, size));
}

bool readifready(void *buff, pipe_t _pipe, size_t size) {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(_pipe.accesses[READ], &set);
    if (select(FD_SETSIZE, &set, NULL, NULL, ACCEPTABLE_WAITING_TIME)) {
        readfrm(buff, _pipe, size);
        return true;
    }
    return false;
}

bool writeifready(void *buff, pipe_t _pipe, size_t size) {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(_pipe.accesses[WRITE], &set);
    if (select(FD_SETSIZE, NULL, &set, NULL, ACCEPTABLE_WAITING_TIME)) {
        writeto(buff, _pipe, size);
        return true;
    }
    return false;
}



// Reversed for sync with father.

void time_process(pipe_t r, pipe_t w) {
    CLOSE_READ(w);
    CLOSE_WRITE(r);
    unsigned int start;
    LOWCOST_INFO info = 0;
    readfrm(&start, r, sizeof(unsigned int));
    time_t starting_time = time(NULL);
    while(start != 0) {
        time_t current_time = time(NULL);

        if (readifready(&info, r, sizeof(LOWCOST_INFO))) {
            if (info) exit(EXIT_SUCCESS);
        }
        if (starting_time < current_time && current_time - starting_time == 1) {
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

void fetch_time(pipe_t r, unsigned int *time) {
    readifready(time, r, sizeof(unsigned int));
}


LOWCOST_INFO process_mode_exec(Screen screen) {
    erase();
    Board board;
    INIT_COLORS;
    INIT_BOARD(board, screen);
    endwin();

    // Processing for real
    pipe_t *arr = create_pipes(2, "writetime", "readtime");

    pid_t ptime = fork();

    if (ptime == -1) return 2;
    else if (ptime == 0) {
        time_process(findpn(2, arr, "writetime"), findpn(2, arr, "readtime"));
    }
    else {
        // Logic
        pipe_t wt = findpn(2, arr, "writetime");
        pipe_t rt = findpn(2, arr, "readtime");
        CLOSE_READ(wt);
        CLOSE_WRITE(rt);
        writeto(&(board.max_time), wt, sizeof(unsigned int));

        // Game
        Bar lf = create_life_bar(&board);
        Bar tm = create_time_bar(&board);

        while(1) {
            fetch_time(rt, &(board.time_left));
            calculate_life_bar(&lf, &board);
            calculate_time_bar(&tm, &board);
            update_graphics(&board, lf, tm);
            if (board.time_left == 0) break;
        }
        return 2;
    }
}



