#define ITEMSZ 32

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
    int relevance; /*higher is better*/
    struct SearchResults * next;
} SearchResults;

