
/*
This program includes Customer billing which manages inventory for a small grocery shop owner.
Copyright (C) 2016 Shubham Singh sshubham37@ymail.com

 *This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include "billing.h"


Items *inventory = NULL, *bill = NULL;

/*Returns a printable string that is suitable for menu item or bill item*/
char* menuAndItemPrint(Item* i, int selected);
/*returns string for inventory item*/
char* inventoryItemPrint(Item* i, int selected);

/*
  create and return a new Item
  name is a pre-malloced string with the name of the item
  quantity is  item quantity
  cost is item unit cost
 */
Item* newItem(char *name, int quantity, float cost) {
    Item *item = malloc(sizeof (Item));
    item->name = name;
    item->quantity = quantity;
    item->unitCost = cost;
    return item;
}

void appendItem(Items *items, Item *item) { /*append item*/
    Item **appendHere = &items->itemList;
    while (*appendHere) {
        appendHere = &((*appendHere)->next);
    }
    *appendHere = item;
}
/*Load inventory from file*/
Items* loadInventory(FILE *data) {
    if (!data) {
        return NULL;
    }
    Items *inv = malloc(sizeof (Items));

    char *name;
    int qtt;
    float cost;

    while (fscanf(data, "%ms%d%f", &name, &qtt, &cost) > 2) {
        Item* itm = newItem(name, qtt, cost);
        appendItem(inv, itm);
    }

    return inv;
}

void printInventory(Items *inv) { /*Printing inventory Function*/
    if (!inv || !inv->itemList) {
        printw("Inventory empty");
        return;
    }
    attron(A_BOLD | COLOR_PAIR(2));
    mvprintw(0, 0, "Inventory Items");
    attron(COLOR_PAIR(1));
    int nameWidth = COLS - 20;
    mvprintw(1, 0, "%-*sQuantity Unit Cost\n", nameWidth, "Item Name");
    attroff(A_BOLD);
    Item *itm = inv->itemList;
    while (itm) {
        printw("%-*s%-8d %.2f\n", nameWidth, itm->name, itm->quantity, itm->unitCost);
        itm = itm->next;
    }
}

int fuzzyMatch(char* pattern, char*string) { /* String based Matching query function */
    int match = 0;
    for (; *pattern; pattern++) {
        for (; *string; string++) {
            if (tolower(*pattern) == tolower(*string)) {
                match++;
                break;
            }
        }
    }
    return match;
}

void fuzzyMatchPrint(char* pattern, char* toPrint) { /* Print string */
    for (; *pattern; pattern++) {
        for (; *toPrint; toPrint++) {
            if (tolower(*toPrint) == tolower(*pattern)) {
                addch(*toPrint | A_BOLD);
                toPrint++;
                break;
            }
            addch(*toPrint);
        }
    }
    while (*toPrint) {
        addch(*toPrint);
        toPrint++;
    }
}

int getItemsMatching(Items *inv, char *name, SearchResults **searchResults) {

    SearchResults *results = NULL;
    if (!name || name[0] == 0) {
        name = "";
    }
void appendSearchResult(Item* item, int relevance) {  /*descending insertion sort on relevance*/
       
        SearchResults *sr = malloc(sizeof (SearchResults));
        sr->item = item;
        sr->relevance = relevance;

        SearchResults **insertHere = &results;
        while (*insertHere) {
            if ((*insertHere)->relevance < relevance) {
                sr->next = (*insertHere);
                *insertHere = sr;
                return;
            }
            insertHere = &((*insertHere)->next);
        }
        sr->next = NULL;
        *insertHere = sr;
}

    int count = 0;
    Item *itm = inv->itemList;
    while (itm) {
        int matching = fuzzyMatch(name, itm->name);
        if (matching || name[0] == 0) {
            appendSearchResult(itm, matching);
            count++;
        }
        itm = itm->next;
    }
    *searchResults = results;
    return count;
}


/*
  Displays a item selection window where in the user searches the items by typing and selects using arrow keys and enter
  inv is the inventory to which search must be restricted to
  return the particular item selected by the user, null if no selection
 */
Item *chooseItem(Items *inv, int options, char* (*getString) (Item * itm, int selected)) { /*choose items from inventory*/

    char matchWith[32] = {0};
    int x = 0, i = 0;
    int choice = -1;
    int resultCount = 0;
    SearchResults *results = NULL;
    Item *selectedItem = NULL;

    void displaySearchScreen() {  /*draw search box screen*/
       
        color_set(2, NULL);
        mvchgat(0, 0, -1, A_BOLD, COLOR_PAIR(2), NULL);
        /*when search string is empty we display prompt text for helping the user*/
        char * searchBoxContents = matchWith[0] == 0 ? "Search items here" : matchWith;
        printw("%-*s", COLS, searchBoxContents);
        clrtobot();

        /*draw search results*/
        color_set(1, NULL);
        SearchResults *sr = results;
        if (!sr) {
            mvchgat(0, 0, -1, A_NORMAL, COLOR_PAIR(3), NULL);
            move(1, 0);
            printw("No Items Found! Press End to go back or type a new search query\n");
            return;
        }
        int index = 0;
        for (; sr; sr = sr->next, index++) {
            if (index < choice) {
                continue;
            }
            color_set(1, NULL);
            if (choice == index) {
                selectedItem = sr->item;
            }
            char *toPrint = getString(sr->item, choice == index);
            fuzzyMatchPrint(matchWith, toPrint);
            move(getcury(stdscr)+1 , 0);
        }
        refresh();
    }

void deleteResults() { /*delete the old results*/
        
        SearchResults *sr = results;
        while (sr) {
            SearchResults *toDel = sr;
            sr = sr->next;
            free(toDel);
        }
}

void updateResults() { /*Get new results*/
        deleteResults();
        
        resultCount = getItemsMatching(inv, matchWith, &results);
        choice = 0;
        selectedItem = NULL;
}
        updateResults();
        do {
            switch (x) {
                case KEY_DOWN:
                    choice++;
                    if (choice >= resultCount) {
                        choice = resultCount - 1;
                    }
                    break;
                case KEY_UP:
                    choice--;
                    if (choice < 0) {
                        choice = 0;
                    }
                    break;
                case KEY_RIGHT: if (options & EDITABLE && selectedItem) {
                        selectedItem->quantity++;
                    }
                    break;
                case KEY_LEFT: if (options & EDITABLE && selectedItem && selectedItem->quantity > 1) {
                        selectedItem->quantity--;
                    }
                    break;
                case KEY_END: deleteResults();
                    return NULL;
                case '\n':
                case '\r':
                case KEY_ENTER: deleteResults();
                    return selectedItem;
                case KEY_BACKSPACE:
                {
                    if (i > 0) {
                        i--;
                        matchWith[i] = 0;
                    }
                    updateResults();
                    break;
                }
                case KEY_RESIZE: break;
                default:
                {
                    if (i < 32 && isgraph(x)) {
                        
                        matchWith[i++] = x;
                        updateResults();
                    }
                }
            }
            displaySearchScreen(choice);
        } while (x = getch());
    }

/*Static billing options*/
Item checkoutMenu = {"Checkout", 1, 1, NULL};
Item addMenu = {"Add New Item", 0, 0, &checkoutMenu};

void billWindow() { /*Adding new items and checkout Window function*/
    Items bill = {&addMenu};
    do {
        Item* chosen = chooseItem(&bill, NON_EDITABLE | EDITABLE, menuAndItemPrint);
        if (chosen == &addMenu) {
            Item* itmfrominv = chooseItem(inventory, NON_EDITABLE, inventoryItemPrint);
            if (itmfrominv && itmfrominv->quantity > 0) {
                Item* billItem = newItem(itmfrominv->name, 1, itmfrominv->unitCost);
                appendItem(&bill, billItem);
            }
        } else if (chosen == &checkoutMenu) {
            float total = 0;
            Item* i = checkoutMenu.next;
            while (i) { 
                total+=i->quantity*i->unitCost;
                i = i->next;
            }
            mvprintw(LINES-1, 0 ,"Total bill $%.2f", total);
            getch();
            return;
        } else if (chosen != NULL) {
            
        }
    } while (1);
}


Item exitMenu = {"Exit", 3, 3, NULL};
Item billMenu = {"New Bill", 2, 2, &exitMenu};
Item viewInventoryMenu = {"View Inventory", 1, 1, &billMenu};
Items menuOptions = {&viewInventoryMenu};

void menuWindow() { /*Menu Window Function*/
    do {
        Item* option = chooseItem(&menuOptions, NON_EDITABLE, menuAndItemPrint);
        if (option == &exitMenu) {
            return;
        } else if (option == &viewInventoryMenu) {
            chooseItem(inventory, NON_EDITABLE, inventoryItemPrint);
        } else if (option == &billMenu) {
            billWindow();
        }
    } while (1);
}


char printBuffer[256] = {0};

char* menuAndItemPrint(Item *i, int selected) { /*Returns a printable string that is suitable for menu item or bill item*/
    if (!i) {
        return "";
    }
    if (i == &checkoutMenu ||
            i == &addMenu ||
            i == &exitMenu ||
            i == &billMenu ||
            i == &viewInventoryMenu) {
        strcpy(printBuffer + 1, i->name);
    } else
        snprintf(printBuffer, 256, " %-*s%-5d x $%-5.2f= $%-8.2f", COLS - 30, i->name, i->quantity, i->unitCost, i->unitCost * i->quantity);
    if (selected) {
        printBuffer[0] = '>';
    } else printBuffer[0] = ' ';
    return printBuffer;
}

char* inventoryItemPrint(Item* i, int selected) { /*returns string for inventory item*/
    if (!i) {
        return "";
    }
    if (i->quantity == 0) {
        snprintf(printBuffer, 256, " %-*s<Out of Stock>", COLS - 22, i->name); /*snprintf is particularly useful for avoiding repetition of formatted string */
    } else snprintf(printBuffer, 256, " %-*s%-5d @ $%-6.2f/unit", COLS - 22, i->name, i->quantity, i->unitCost);
    if (selected) {
        printBuffer[0] = '>';
    } else printBuffer[0] = ' ';
    return printBuffer;
}


int main(int argc, char** argv) { /* Main Function */
    initscr(); /* Start ncurses mode */
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
        printw("Could not find inventory database");
        getch();
    }

    inventory = loadInventory(dataFile);
    menuWindow();
    endwin(); /* End ncurses mode*/ 
    return (EXIT_SUCCESS);
}

