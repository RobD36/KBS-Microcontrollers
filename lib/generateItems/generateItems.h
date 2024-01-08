#ifndef LEVELMAKER_H
#define LEVELMAKER_H
#include "items.h"
#include "Shared.h"
#include "time.h"

Item *generateItems(long time);
bool checkCollision(Item item, Item itemsArray[], int amountOfItems);

#endif