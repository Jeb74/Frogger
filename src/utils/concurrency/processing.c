#include "../../headers/processing.h"

// Reversed for sync with father.

void time_process(int _read[2], int _write[2]) {
    close(_read[WRITE]);
    close(_write[READ]);
    unsigned int start;
    HANDLE_ERROR(read(_read[READ], &start, sizeof(unsigned int)));

    time_t starting_time = time(NULL);
    fd_set ready_pipes, current_pipes;

    FD_ZERO(&current_pipes);
    FD_SET(_read[READ], &current_pipes);

    char str[30];
    while(start != 0) {
        time_t current_time = time(NULL);

        ready_pipes = current_pipes;

        if (select(FD_SETSIZE, &ready_pipes, NULL, NULL, ACCEPTABLE_WAITING_TIME)) exit(EXIT_SUCCESS);
        if (starting_time < current_time && current_time - starting_time == 1) {
            starting_time = current_time;
            start--;
            HANDLE_ERROR(write(_write[WRITE], &start, sizeof(unsigned int)));
            sprintf(str, "%i btl\n", start);
            mvaddstr(1, 0, str);
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

void fetch_time(fd_set pipes, int pipe, unsigned int *time) {
    if (select(FD_SETSIZE, &pipes, NULL, NULL, ACCEPTABLE_WAITING_TIME)) {
        HANDLE_ERROR(read(pipe, time, sizeof(unsigned int)));
    }
}

LOWCOST_INFO process_mode_exec(Screen screen) {
    erase();
    Board board;
    INIT_COLORS;
    INIT_BOARD(board, screen);

    // Processing for real
    int wtmpipe[2];
    int rtmpipe[2];

    HANDLE_ERROR(pipe(wtmpipe)+1);

    HANDLE_ERROR(pipe(rtmpipe)+1);

    pid_t ptime = fork();

    if (ptime == -1) return 2;
    else if (ptime == 0) {
        time_process(wtmpipe, rtmpipe);
    }
    else {
        close(wtmpipe[READ]);
        close(rtmpipe[WRITE]);
        
        HANDLE_ERROR(write(wtmpipe[WRITE], &(board.max_time), sizeof(unsigned int)));

        fd_set current_pipes;
        FD_ZERO(&current_pipes);
        FD_SET(rtmpipe[READ], &current_pipes);
        char str[30];
        while(1) {
            fetch_time(current_pipes, rtmpipe[READ], &(board.time_left));
            update_graphics(&board);
            if (board.time_left == 0) break;
        }
        waitpid(ptime, NULL, 0);
        return 2;
    }
}



