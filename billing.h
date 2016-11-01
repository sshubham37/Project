#define NON_EDITABLE 0
#define EDITABLE 1

typedef struct Item {
    char *name;
    int quantity;
    float unitCost;
    struct Item *next;
} Item;

typedef struct Items {
    Item *itemList;
} Items;

typedef struct SearchResults {
    Item *item;
    int relevance; 
    struct SearchResults * next;
} SearchResults;

