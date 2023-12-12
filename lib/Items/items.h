#ifndef ITEMS_H
#define ITEMS_H

#include <Arduino.h>

enum ItemType
{
    STONE,
    GOLD,
    DIAMOND
};

class Item
{
public:
    Item(ItemType type, int x, int y, int size);

    int size;
    int x;
    int y;
    int value;
    ItemType type;
};

#endif