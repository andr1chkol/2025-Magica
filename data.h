#ifndef DATA_H
#define DATA_H

#define NUMBER_OF_ITEMS 16
#define MAX_NAME 100
#define MIN_ARMY 1
#define MAX_ARMY 5

#define MAP_WIDTH 40
#define MAP_HEIGHT 20

#define SPRITE_HEIGHT 4

#define ERR_UNIT_COUNT "ERR_UNIT_COUNT"
#define ERR_ITEM_COUNT "ERR_ITEM_COUNT"
#define ERR_WRONG_ITEM "ERR_WRONG_ITEM"
#define ERR_SLOTS "ERR_SLOTS"
#define MAX_SLOTS 2


typedef struct item {
    char name[MAX_NAME + 1];
    int att;
    int def;
    int slots;
    int range;
    int radius;
} ITEM;

typedef struct unit {
    char name[MAX_NAME + 1];
    const ITEM *item1;
    const ITEM *item2;
    int hp;
    int y;
    const char* sprite[SPRITE_HEIGHT];
    int color_pair;

} UNIT;

extern const ITEM items[NUMBER_OF_ITEMS];
extern const char* default_sprite[SPRITE_HEIGHT];

#endif


