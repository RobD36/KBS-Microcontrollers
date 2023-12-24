#include "levelMaker.h"

//check if item collides with other items
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

//make level with given amount of items
Item* makeLevel(int amountOfItems)
{
    Item* items = new Item[amountOfItems];
    for (int i = 0; i < amountOfItems; i++)
    { 
        int randomType = random(0, 100);
        enum ItemType type;
        int size;
        int y;
        if (randomType < 50) // 50% chance of stone
        {
            type = STONE;
            size = random(15, 50);
            y = random(110, 180); // stone spawns the highest
        }
        else if (randomType < 85) // 35% chance of gold
        {
            type = GOLD;
            size = random(15, 50);
            y = random(130, 200); // gold spawns lower than stone
        }
        else // 15% chance of diamond
        {
            type = DIAMOND;
            size = 5;
            y = random(200, 230); // diamond can only be on lowest level
        }
        int x = random(0, 320-size); // random x position witin screen

        Item item(type, x, y, size); 
        if (checkCollision(item, items, i)) {
            i--; // retry creating item
        } else {
            items[i] = item; // add item to array
        }
    }
    return items;
}