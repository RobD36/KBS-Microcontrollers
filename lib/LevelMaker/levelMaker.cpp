#include "levelMaker.h"

bool checkCollision(Item item, Item itemsArray[], int amountOfItems)
{
    for (int i = 0; i < amountOfItems; i++)
    {
        if (item.x < itemsArray[i].x + itemsArray[i].size &&
            item.x + item.size > itemsArray[i].x &&
            item.y < itemsArray[i].y + itemsArray[i].size &&
            item.y + item.size > itemsArray[i].y)
        {
            return true;
        }
    }
    return false;
}

Item* makeLevel(int amountOfItems)
{
    Item* items = new Item[amountOfItems];
    for (int i = 0; i < amountOfItems; i++)
    { 
        int randomType = random(0, 100);
        enum ItemType type;
        int size;
        int y;
        if (randomType < 50)
        {
            type = STONE;
            size = random(15, 50);
            y = random(110, 180);
        }
        else if (randomType < 80)
        {
            type = GOLD;
            size = random(15, 50);
            y = random(130, 200);
        }
        else
        {
            type = DIAMOND;
            size = 5;
            y = random(200, 230);
        }
        int x = random(0, 320-size);
        Item item(type, x, y, size);
        if (checkCollision(item, items, i)) {
            i--; // retry creating item
        } else {
            items[i] = item;
        }
    }
    return items;
}