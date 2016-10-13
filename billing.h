
typedef struct Item {
    char *name;
    int quantity;
    float unitCost;
    struct Item *next;
} Item;

typedef struct Items {
    Item *itemList;
} Items;



