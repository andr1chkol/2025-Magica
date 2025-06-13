#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "print.h"
#include "data.h"




int main_menu () { //Show main menu
  const char* options[] = {"NEW GAME", "LOAD GAME", "INSTRUCTION", "QUIT"};
  int option = 0;

  const char* sword_art[] = {
      "     _ ",
      "    (_)",
      "    |_|",
      "    |_|",
      "    |_|",
      "    |_|",
      "    |_|",
      "o=========o",
      "    | |",
      "    | |",
      "    | |",
      "    | |",
      "    | |",
      "    | |",
      "    | |",
      "    | |",
      "    | |",
      "    | |",
      "    | |",
      "    | |",
      "    | |",
      "    | |",
      "    | |",
      "    | |",
      "    \\ /"
  };

  initscr();
  start_color();
  cbreak();
  noecho();
  curs_set(FALSE);
  keypad(stdscr, TRUE);

  int height, width;
  getmaxyx(stdscr, height, width);

  int menu_height = 25;
  int menu_width = 50;
  int start_y = (height - menu_height) / 2;
  int start_x = (width - menu_width) / 2;

  WINDOW* menu_win = newwin(menu_height, menu_width, start_y, start_x);

  refresh();

  while (1) { //Loop for choosing mode
    werase(menu_win);
    box(menu_win, 0, 0);

    for (int i = 0; i < 25; i++) {
      mvprintw(start_y + i, start_x - 20, "%s", sword_art[i]);
    }
    const char* title = {"----- M A G I C A -----"};
    wattron(menu_win, COLOR_PAIR(2) | A_BOLD);
    mvwprintw(menu_win, 1, (menu_width - strlen(title)) / 2, title);
    wattroff(menu_win, COLOR_PAIR(2) | A_BOLD);

    int menu_items = 4;
    int row = (menu_height - menu_items) / 2;

    for (int i = 0; i < 4; i++) { //Arrows for navigation
      if (i == option) {
        wattron(menu_win, COLOR_PAIR(2) | A_BOLD);
        mvwprintw(menu_win, row + i, (menu_width - strlen(options[i])) / 2, options[i]);
        wattroff(menu_win, COLOR_PAIR(2) | A_BOLD);
      } else {
        wattron(menu_win, COLOR_PAIR(1));
        mvwprintw(menu_win, row + i, (menu_width - strlen(options[i])) / 2, options[i]);
        wattroff(menu_win, COLOR_PAIR(1));
      }
    }
    wrefresh(menu_win);
    int key = getch();

    if ((key == KEY_UP || key == 'w' || key == 'W') && option > 0) option--;
    else if ((key == KEY_DOWN || key == 's' || key == 'S') && option < 3) option++;
    else if (key == '\n') break;
  }
  delwin(menu_win);
  endwin();
  return option;
}

//Drawing army zone
void draw_army_window(WINDOW* win, UNIT* army, int count, DAMAGE* events, int event_count, int is_left) {
    werase(win);
    box(win, 0, 0);

    int spacing = 14;
    int win_width = getmaxx(win);
    int win_height = getmaxy(win);

    int total_width = spacing * count;
    int start_x = (win_width - total_width) / 2;
    int start_y = (win_height - (SPRITE_HEIGHT + 6)) / 2;

    int color_pair = is_left ? 2 : 3;

    for (int i = 0; i < count; i++) {
        int index = is_left ? (count - 1 - i) : i;
        int x = start_x + i * spacing;

        wattron(win, COLOR_PAIR(color_pair)|A_BOLD);

        for (int row = 0; row < SPRITE_HEIGHT; row++)
            mvwprintw(win, start_y + row, x, "%s", army[index].sprite[row]);

        mvwprintw(win, start_y + SPRITE_HEIGHT,     x, "%s", army[index].name);
        mvwprintw(win, start_y + SPRITE_HEIGHT + 1, x, "HP: %d", army[index].hp);
        mvwprintw(win, start_y + SPRITE_HEIGHT + 2, x, "1: %s", army[index].item1->name);
        if (army[index].item2)
            mvwprintw(win, start_y + SPRITE_HEIGHT + 3, x, "2: %s", army[index].item2->name);


        wattroff(win, COLOR_PAIR(color_pair)|A_BOLD);
    }
    wrefresh(win);
}

//Show all items which are in data.c
void items_table(WINDOW* win) {
    werase(win);
    box(win, 0, 0);

    mvwprintw(win, 1, 2, "Items:");
    for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
        mvwprintw(win, 3 + i, 2, "- %s (ATT: %d, DEF: %d, SLOTS:%d)",
                  items[i].name,
                  items[i].att,
                  items[i].def,
                  items[i].slots);
    }
    wrefresh(win);
}

