#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "data.h"

void save_game(const char* filename, UNIT* army1, int count1, UNIT* army2, int count2);
UNIT* load_game(const char* filename, int* count1, int* count2, UNIT** army2_out);
#endif //SAVE_LOAD_H
