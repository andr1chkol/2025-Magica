#ifndef BATTLE_H
#define BATTLE_H

#include "data.h"
#include <ncurses.h>

typedef struct {
    int army;
    const char* attacker_name;
    const char* item;
    const char* defender_name;
    int damage;
    int item_index;
} DAMAGE;

UNIT* input_army_ncurses(int* army_count, int army_side, WINDOW* win, WINDOW* items_win);
const ITEM* item_find(const char* search_item);
void update_army(UNIT* army, int* army_count, const int* damage);
void attack_army(const UNIT* army_attack, int attack_count, const UNIT* army_deffence, int deffence_count, int* damage,
                 DAMAGE** events, int *event_count, int *event_size, int army_id );
int is_game_over(int army1_count, int army2_count, int max_y, int max_x);



#endif //BATTLE_H