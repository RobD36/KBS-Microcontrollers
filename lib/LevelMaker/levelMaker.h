#ifndef LEVELMAKER_H
#define LEVELMAKER_H
#include "items.h"

Item* makeLevel(int amountOfItems);
bool checkCollision(Item item, Item itemsArray[], int amountOfItems);

#endif