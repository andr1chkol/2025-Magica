# Magica

**Magica** is a turn-based console strategy game written in C using the `ncurses` library.  
The player creates two armies, equips units with items, and watches them battle turn by turn.

---

## About

This game was developed as a personal project and tested on **macOS**.  
The core mechanics are finished, including gameplay, save/load, and a simple ncurses interface.  
Some visual features are not implemented, but the main gameplay loop is complete.

---

## Features

- Console UI using `ncurses`
- Two custom-built armies
- Item system with slot limitations
- Turn-based damage simulation
- Save and load functionality
- Color-coded teams and basic unit sprites

---

## Compilation & Running

### Requirements:

- GCC or Clang
- `ncurses` library
- Unix-based system (macOS/Linux)

### Manual compilation:

```
bash
gcc main.c data.c save_load.c battle.c print.c -lncurses -o battle
./battle
```

## Important: Run in a full-screen terminal window for best experience.
