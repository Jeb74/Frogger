#ifndef FROGGER_DATA_H
#define FROGGER_DATA_H

#include "structures.h"

#define FROG_LIVES 3

#define FROG_HEIGHT 2
#define FROG_WIDTH 2

#define FROG_VELOCITY_X 1
#define FROG_VELOCITY_Y 2

#define CAR_MIN_SIZE 4
#define CAR_MAX_SIZE 6

#define TRUCK_MIN_SIZE 7
#define TRUCK_MAX_SIZE 10

#define LOG_MIN_WIDTH 4
#define LOG_MAX_WIDTH 6

#define HIGHWAY_LANES 5
#define HIGHWAY_LANE_HEIGHT FROG_HEIGHT

#define RIVER_LANES 5
#define RIVER_LANE_HEIGHT FROG_HEIGHT

#define LAWN_LANES 2
#define LAWN_LANE_HEIGHT FROG_HEIGHT

#define SCORE_HEIGHT 5
#define SCORE_WIDTH 3
#define SCORE_LENGTH 20

#define SCORE_0 "111 101 101 101 111"
#define SCORE_1 "010 110 010 010 111"
#define SCORE_2 "111 001 111 100 111"
#define SCORE_3 "111 001 111 001 111"
#define SCORE_4 "001 011 101 111 001"
#define SCORE_5 "111 100 111 001 111"
#define SCORE_6 "111 100 111 101 111"
#define SCORE_7 "111 001 010 100 100"
#define SCORE_8 "111 101 111 101 111"
#define SCORE_9 "111 101 111 001 001"

static const char *SCORES[] = {
    SCORE_0,
    SCORE_1,
    SCORE_2,
    SCORE_3,
    SCORE_4,
    SCORE_5,
    SCORE_6,
    SCORE_7,
    SCORE_8,
    SCORE_9
};

static ExecutionMode exm;

void set_exm(ExecutionMode exm);
ExecutionMode get_exm();

#endif // FROGGER_DATA_H
