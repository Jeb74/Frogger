#ifndef FROGGER_THREADING_H
#define FROGGER_THREADING_H

#ifndef FROGGER_STRUCTURES_H
#include "structures.h"
#endif // FROGGER_STRUCTURES_H
#include "structures.h"

void create_thread(pthread_t *thread, void *manager, Package args);
void create_threads(pthread_t *threads, int num_threads, void *manager, Package args);

int cancel_thread(pthread_t *thread);
int *cancel_threads(pthread_t *threads, int num_threads);

#endif //FROGGER_THREADING_H
