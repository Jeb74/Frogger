#include "file_manager.h"

static char **files = NULL;

static int getFileId() {
}

static char *getFileById(int id) {
}

void initializeFileList() {
    if (!files) {

    }
}

void save_game(int id, game game) {
    FILE *file = fopen(getFileById(id), "wb");
    fwrite(&game, sizeof(game), 1, file);
}

int open_saving(game *game) {
    int id = getFileId();
    FILE *file = fopen(getFileById(id), "rb");
    fread(game, sizeof(game), 1, file);
    return id;
}