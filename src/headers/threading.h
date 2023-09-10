#ifndef FROGGER_THREADING_H
#define FROGGER_THREADING_H

#include "libraries.h"

void create_thread(pthread_t *thread, void *manager, ThreadArgs *args);
void create_threads(pthread_t *threads, int num_threads, void *manager, ThreadArgs *args);

int cancel_thread(pthread_t *thread);
int *cancel_threads(pthread_t *threads, int num_threads);

#endif //FROGGER_THREADING_H
