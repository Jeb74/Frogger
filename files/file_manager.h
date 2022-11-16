#ifndef FROGGER_FILE_MANAGER_H
#define FROGGER_FILE_MANAGER_H

#ifndef FROGGER_GENERAL_H
#include "../utils/general.h"
#endif

/*
 * Le varie path utilizzate per la gestione dei files.
 */
#define SAVE_PATH "../saves/"
#define SAVE_PATH_LEN strlen(SAVE_PATH)

// Le directory da ignorare.
#define IGNORE_DIR '.'

void save_game(int id, game game);

#endif //FROGGER_FILE_MANAGER_H
