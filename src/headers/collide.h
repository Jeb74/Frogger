#ifndef FROGGER_COLLIDE_H
#define FROGGER_COLLIDE_H

#include "structures.h"

void check_collisions(Board *board, EntityQueue *eq);
int *get_bottom_right_corner(Entity *entity);

#endif