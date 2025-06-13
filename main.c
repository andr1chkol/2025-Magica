#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "data.h"
#include "battle.h"
#include "print.h"
#include "save_load.h"

int main() {
    //Ncurses
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();

    //Colours
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);


    int event_count = 0;
    int event_size = 100;
    DAMAGE* events = malloc(sizeof(DAMAGE) * event_size); //Allocation of damage array
    if (!events) {
        endwin();
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }


    while (1) {
        int choice = main_menu();
        if (choice == 3) break;
        if (choice == 2) {
            //Making window for instruction
            int h = 20, w = 60;
            int y = (LINES - h) / 2;
            int x = (COLS - w) / 2;
            WINDOW* info = newwin(h, w, y, x);
            box(info, 0, 0);
            wattron(info, A_BOLD);
            mvwprintw(info, 1, 2, "INSTRUCTIONS");
            wattroff(info, A_BOLD);
            mvwprintw(info, 3, 2, "- Enter unit name and items.");
            mvwprintw(info, 4, 2, "- Max 2 items per unit. Slot limit: %d.", MAX_SLOTS);
            mvwprintw(info, 5, 2, "- Items shown in right panel during input.");

            mvwprintw(info, 7, 2, "During battle:");
            mvwprintw(info, 8, 4, "ENTER  - advance to next round");
            mvwprintw(info, 9, 4, "S      - save game to file");
            mvwprintw(info,10, 4, "Colors - red = army 1, blue = army 2");

            mvwprintw(info,12, 2, "To start a new game, return to menu.");
            mvwprintw(info,13, 2, "To load previous save, choose 'Load Game'.");

            mvwprintw(info,15, 2, "Press any key to return to menu...");
            wrefresh(info);
            wgetch(info);
            delwin(info);
            clear();
            refresh();
            continue;
        }

        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);
        int input_w = max_x / 2;
        int items_w = max_x - input_w;
        int input_h = max_y - 2;
        WINDOW* input_win = newwin(input_h, input_w, 1, 0);
        WINDOW* items_win = newwin(input_h, items_w, 1, input_w);

        int army_1_count = 0, army_2_count = 0;
        UNIT *army_1 = NULL, *army_2 = NULL;

        //Choosing mode
        if (choice == 0) {
            //GAME
            army_1 = input_army_ncurses(&army_1_count, 1, input_win, items_win);
            if (!army_1) {
                endwin();
                fprintf(stderr, "Army 1 allocation failed.\n");
                exit(1);
            }
            army_2 = input_army_ncurses(&army_2_count, 2, input_win, items_win);
            if (!army_2) {
                endwin();
                fprintf(stderr, "Army 2 allocation failed.\n");
                free(army_1);
                exit(1);
            }
        } else if (choice == 1) {
            //SAVE LOAD
            army_1 = load_game("save.txt", &army_1_count, &army_2_count, &army_2);
            if (!army_1 || !army_2) {
                int h = 5, w = 30;
                int y = (LINES - h) / 2;
                int x = (COLS - w) / 2;
                WINDOW* fail_win = newwin(h, w, y, x);
                box(fail_win, 0, 0);
                mvwprintw(fail_win, 2, (w - 20) / 2, "Failed to load game!");
                wrefresh(fail_win);
                napms(1500);
                delwin(fail_win);
                clear(); refresh();
                continue;
            }
        }

        delwin(input_win);
        delwin(items_win);
        clear(); refresh();

        int half_width = max_x / 2;
        int win_height = max_y - 4;
        WINDOW* win_left  = newwin(win_height, half_width, 2, 0);
        WINDOW* win_right = newwin(win_height, max_x - half_width, 2, half_width);

        int round = 1;
        while (1) {
            //Battle loop
            event_count = 0;
            int* dmg1 = calloc(army_1_count, sizeof(int));
            int* dmg2 = calloc(army_2_count, sizeof(int));

            werase(win_left); box(win_left, 0, 0);
            werase(win_right); box(win_right, 0, 0);

            draw_army_window(win_left, army_1, army_1_count, events, event_count, 1);
            draw_army_window(win_right, army_2, army_2_count, events, event_count, 0);

            mvprintw(0, 2, "ENTER = next round | S = save | Round %d", round);
            refresh();

            int ch = wgetch(stdscr); // Enter for continue or S for save
            if (ch == 's' || ch == 'S') {
                save_game("save.txt", army_1, army_1_count, army_2, army_2_count);
                mvprintw(1, 2, "Game saved!");
                refresh(); napms(1000);
                continue;
            }
            if (ch != '\n') continue;

            attack_army(army_1, army_1_count, army_2, army_2_count, dmg1, &events, &event_count, &event_size, 1);
            attack_army(army_2, army_2_count, army_1, army_1_count, dmg2, &events, &event_count, &event_size, 2);
            update_army(army_1, &army_1_count, dmg2);
            update_army(army_2, &army_2_count, dmg1);
            free(dmg1); free(dmg2);
            round++;

            if (is_game_over(army_1_count, army_2_count, max_y, max_x)) break; //Checking if end of game
        }

        delwin(win_left); delwin(win_right);
        free(army_1); free(army_2);
        clear(); refresh();
    }

    //Cleaning
    free(events);
    endwin();
    return 0;
}