#ifndef ITEMS_H
#define ITEMS_H

#include <Arduino.h>

enum ItemType
{
    STONE,
    GOLD,
    DIAMOND,
    RAT
};

class Item
{
public:
    Item();
    Item(ItemType type, int x, int y, int size);

    int size;
    int x;
    int y;
    int value;
    ItemType type;
};

#endif