#include "file_manager.h"

static char **files = NULL;
static unsigned short filesNumber = 0;
static int getFileId() {
    // Da sincronizzare con la parte grafica
}

static char *getFileById(int id) {
    DCALLOC(fname, char, SAVE_PATH_LEN + strlen(files[id]) + SAVE_PATH_EXT_LEN + 1);
    strcpy(fname, SAVE_PATH);
    strcat(fname, files[id]);
    strcat(fname, SAVE_PATH_EXT);
    return fname;
}

void initializeFileList() {
    if (!files) {

        struct dirent *d;

        char *res = NULL;

        DIR *dir = opendir(SAVE_PATH); CRASH_IF_NULL(dir)
        DCALLOC(fileNames, char *, 1)

        /* Prende ogni file dalla directory di salvataggio. */
        while ((d = readdir(dir)) != NULL)
        {
            /* Ignora ogni file che inizia per "." e "..", ovvero current-folder e previous-folder. */
            if (d->d_name[0] != IGNORE_DIR)
            {
                unsigned size;
                char **name = strcut(d->d_name, '.', &size);
                ALLOC(fileNames[filesNumber], char, strlen(name[0]) + 1, CALLOC)
                strcpy(fileNames[filesNumber], name[0]);
                DOUBLEFREE(name, size)

                AREALLOC(char *, fileNames, ++filesNumber + 1)
            }
        }
        closedir(dir);
        files = fileNames;
    }
}

void save_game(int id, game game) {
    FILE *file = fopen(getFileById(id), "wb");
    fwrite(&game, sizeof(game), 1, file);
}

int open_saving(game *_game) {
    int id = getFileId();
    FILE *file = fopen(getFileById(id), "rb");
    fread(_game, sizeof(game), 1, file);
    return id;
}