#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "data.h"
#include "battle.h"
#include "print.h"

UNIT* input_army_ncurses(int* army_count, int army_side, WINDOW* input_win, WINDOW* items_win) {
    // Create input form for army units using ncurses
    char buf[16];
    int color_pair = (army_side == 1) ? 2 : 3;

    int h, w;
    getmaxyx(input_win, h, w);

    while (1) {
        werase(input_win);
        box(input_win, 0, 0);
        items_table(items_win);

        wattron(input_win, COLOR_PAIR(color_pair) | A_BOLD);
        mvwprintw(input_win, h/2 - 1, (w - 22)/2, "Enter unit count 1-5: "); //Asking for units count
        wattroff(input_win, COLOR_PAIR(color_pair) | A_BOLD);
        wrefresh(input_win);

        echo();
        wgetnstr(input_win, buf, 5);
        noecho();
        *army_count = atoi(buf);
        if (*army_count >= MIN_ARMY && *army_count <= MAX_ARMY) break;

        mvwprintw(input_win, h/2 + 1, (w - 30)/2, "Invalid number. Press any key...");
        wrefresh(input_win);
        wgetch(input_win);
    }

    UNIT* army = malloc(sizeof(UNIT) * (*army_count));
    if (!army) return NULL;

    for (int i = 0; i < *army_count; i++) {
        char input[256], item1[32], item2[32] = "";
        while (1) {
            werase(input_win);
            box(input_win, 0, 0);
            items_table(items_win);

            wattron(input_win, COLOR_PAIR(color_pair) | A_BOLD);
            mvwprintw(input_win, h/2 - 1, (w - 26)/2, "Enter name for unit %d: ", i + 1); //Asking for unit name
            wattroff(input_win, COLOR_PAIR(color_pair) | A_BOLD);
            wrefresh(input_win);

            echo();
            wgetnstr(input_win, army[i].name, MAX_NAME);
            noecho();
            if (strlen(army[i].name) > 0) break;

            mvwprintw(input_win, h/2 + 1, (w - 30)/2, "Name cannot be empty. Press any key...");
            wrefresh(input_win);
            wgetch(input_win);
        }

        while (1) { //Loop for input items
            werase(input_win);
            box(input_win, 0, 0);
            items_table(items_win);

            wattron(input_win, COLOR_PAIR(color_pair) | A_BOLD);
            mvwprintw(input_win, h/2 - 1, (w - 32)/2, "Enter item1 [item2] for %s: ", army[i].name);
            wattroff(input_win, COLOR_PAIR(color_pair) | A_BOLD);
            wrefresh(input_win);

            echo();
            wgetnstr(input_win, input, 255);
            noecho();

            int count = sscanf(input, "%s %s", item1, item2);
            const ITEM* found1 = item_find(item1);
            const ITEM* found2 = (count == 2) ? item_find(item2) : NULL;

            if (!found1 || (count == 2 && !found2)) {
                mvwprintw(input_win, h/2 + 1, (w - 34)/2, "Invalid item(s). Press any key...");
                wrefresh(input_win);
                wgetch(input_win);
                continue;
            }

            int slots = found1->slots + (found2 ? found2->slots : 0);
            if (slots > MAX_SLOTS) {
                mvwprintw(input_win, h/2 + 1, (w - 34)/2, "Too many slots. Press any key...");
                wrefresh(input_win);
                wgetch(input_win);
                continue;
            }

            army[i].item1 = found1;
            army[i].item2 = found2;
            break;
        }

        army[i].hp = 100;
        army[i].y = i;

        for (int s = 0; s < SPRITE_HEIGHT; s++)
            army[i].sprite[s] = default_sprite[s];
    }
    return army;
}

const ITEM* item_find(const char* search_item) {
    // Find item by name in the global items array; return pointer if found, else NULL
    for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
        if (strcmp(items[i].name, search_item) == 0) {
            return &items[i];
        }
    }
    return NULL;
}

void update_army(UNIT* army, int* army_count, const int* damage) {
    // Apply damage to each unit; remove dead units by shifting survivors to the front
    int alive = 0;
    for (int i = 0; i < *army_count; i++) {
        army[i].hp -= damage[i];
        if (army[i].hp > 0) {
            if (i != alive)
                army[alive] = army[i];
            alive++;
        }
    }
    *army_count = alive;
}

int is_game_over(int army1_count, int army2_count, int max_y, int max_x) {
    //Checking for game end
    if (army1_count == 0 || army2_count == 0) {
        int winner = 0;
        if (army1_count == 0 && army2_count == 0) winner = 0;
        else if (army1_count == 0) winner = 2;
        else if (army2_count == 0) winner = 1;

        int result_win_h = 7;
        int result_win_w = 30;
        int result_y = (max_y - result_win_h) / 2;
        int result_x = (max_x - result_win_w) / 2;

        WINDOW* result_win = newwin(result_win_h, result_win_w, result_y, result_x);
        box(result_win, 0, 0);

        //Showing window with winner
        if (winner == 1) {
            wattron(result_win, COLOR_PAIR(2) | A_BOLD);
            mvwprintw(result_win, 2, (result_win_w - 16) / 2, "WINNER: TEAM 1");
            wattroff(result_win, COLOR_PAIR(2) | A_BOLD);
        }
        else if (winner == 2) {
            wattron(result_win, COLOR_PAIR(3) | A_BOLD);
            mvwprintw(result_win, 2, (result_win_w - 16) / 2, "WINNER: TEAM 2");
            wattroff(result_win, COLOR_PAIR(3) | A_BOLD);
        }
        else {
            wattron(result_win, COLOR_PAIR(1) | A_BOLD);
            mvwprintw(result_win, 2, (result_win_w - 10) / 2, "NO WINNER");
            wattroff(result_win, COLOR_PAIR(1) | A_BOLD);
        }

        mvwprintw(result_win, 4, (result_win_w - 22) / 2, "Press ENTER to exit...");
        wrefresh(result_win);
        while (wgetch(result_win) != '\n') {
            // wait for Enter
        }

        werase(result_win);
        wrefresh(result_win);
        delwin(result_win);

        return 1;
    }

    return 0;
}

void attack_army(const UNIT* attackers, int ac, const UNIT* defenders, int dc, int* dmg,
                 DAMAGE** events, int* ec, int* es, int id) {
    //Simulating attack
    for (int i = 0; i < ac; i++) {
        const ITEM* items[2] = {attackers[i].item1, attackers[i].item2};
        for (int it = 0; it < 2; it++) {
            const ITEM* item = items[it];
            if (!item || item->range < i) continue;  //Checking if item can reach unit

            // Calculate damage
            for (int j = 0; j <= item->radius && j < dc; j++) {
                int def = defenders[j].item1->def + (defenders[j].item2 ? defenders[j].item2->def : 0);
                int damage = item->att - def;
                if (damage < 1) damage = 1;
                dmg[j] += damage;

                if (*ec >= *es) {
                    *es *= 2;
                    *events = realloc(*events, sizeof(DAMAGE) * (*es));
                    if (!*events) exit(1);
                }

                //This is essential for 2nd version of the game
                (*events)[*ec].army = id;
                (*events)[*ec].attacker_name = attackers[i].name;
                (*events)[*ec].item = item->name;
                (*events)[*ec].defender_name = defenders[j].name;
                (*events)[*ec].damage = damage;
                (*events)[*ec].item_index = it;
                (*ec)++;
            }
        }
    }
}