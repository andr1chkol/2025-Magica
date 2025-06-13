#ifndef PRINT_H
#define PRINT_H

#include "data.h"
#include "battle.h"

int main_menu ();
void draw_army_window(WINDOW* win, UNIT* army, int count, DAMAGE* events, int event_count, int is_left);
void items_table(WINDOW* win);


#endif
