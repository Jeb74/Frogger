#ifndef FROGGER_LIBRARIES_H
#define FROGGER_LIBRARIES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include <ncurses.h>

#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "structures.h"
#include "shortcuts.h"

#include "concurrency/threading.h"

#include "../game/data.h"
#include "../game/clock.h"
#include "../game/frog.h"

#endif // FROGGER_LIBRARIES_H
