#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "billing.h"

Items *inventory = NULL, *bill = NULL;

Item* newItem(char *name, int quantity, float cost) {
    Item *item = malloc(sizeof (Item));
    item->name = name;
    item->quantity = quantity;
    item->unitCost = cost;
    return item;
}

void appendItem(Items *items, Item *item) {
    Item **appendHere = &items->itemList;
    while (*appendHere) {
        appendHere = &((*appendHere)->next);
    }
    *appendHere = item;
}

Items* loadInventory(FILE *data) {
    if (!data) {
        return NULL;
    }
    Items *inv = malloc(sizeof (Items));

    char *name;
    int qt;
    float cost;

    while (fscanf(data, "%ms%d%f", &name, &qt, &cost) > 2) {
        Item* itm = newItem(name, qt, cost);
        appendItem(inv, itm);
    }

    return inv;
}

void printInventory(Items *inv) {
    if (!inv || !inv->itemList) {
        printw("Inventory empty");
        return;
    }
    Item *itm = inv->itemList;
    while (itm) {
        printw("%s\t%d\t%f\n", itm->name, itm->quantity, itm->unitCost);
        itm = itm->next;
    }
}
/*Not yet working*/
int main(int argc, char** argv) {
    initscr(); 
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(4, COLOR_BLACK, COLOR_GREEN);
        init_pair(3, COLOR_BLACK, COLOR_RED);
        init_pair(2, COLOR_BLACK, COLOR_WHITE);
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        color_set(1, NULL);
    }

  
    FILE* dataFile = fopen("data.txt", "r");
    if (!dataFile) {
        color_set(3, 0);
        printw("No inventory Database");
        getch();
    }

    inventory = loadInventory(dataFile);

    endwin();
    return (EXIT_SUCCESS);
}

