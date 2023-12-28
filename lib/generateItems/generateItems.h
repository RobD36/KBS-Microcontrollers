#ifndef LEVELMAKER_H
#define LEVELMAKER_H
#include "items.h"

Item* generateItems(int amountOfItems);
bool checkCollision(Item item, Item itemsArray[], int amountOfItems);

#endif