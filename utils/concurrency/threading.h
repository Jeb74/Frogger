#ifndef FROGGER_THREADING_H
#define FROGGER_THREADING_H

#include <pthread.h>
#include "../libraries.h"

void create_thread(pthread_t *thread, void *manager, ThreadArgs *args);
void create_threads(pthread_t *threads, int num_threads, void *manager, ThreadArgs *args);

void cancel_thread(pthread_t *thread);
void cancel_threads(pthread_t *threads, int num_threads);

#endif //FROGGER_THREADING_H
