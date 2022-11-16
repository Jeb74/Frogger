#include "file_manager.h"

static char **files = NULL;
static int getFileId() {
}

static char *getFileById(int id) {
}

void initializeFileList() {
    if (!files) {

        int counter = 0;

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
                ALLOC(fileNames[counter], char, strlen(d->d_name) + 1, CALLOC)

                strcpy(fileNames[counter], d->d_name);
                printf("\t%d |> %s\n", counter, fileNames[counter]);

                AREALLOC(char *, fileNames, ++counter + 1)
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

int open_saving(game *game) {
    int id = getFileId();
    FILE *file = fopen(getFileById(id), "rb");
    fread(game, sizeof(game), 1, file);
    return id;
}