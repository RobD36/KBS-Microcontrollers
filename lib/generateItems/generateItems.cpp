#include "generateItems.h"

// check if item collides with other items
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

// generate items based on current level
Item *generateItems()
{
    int amountOfItems;
    int chanceOfStone;
    int chanceOfGold;

    switch (currentLevel)
    {
    case 1:
        amountOfItems = 5;
        chanceOfStone = 50;
        chanceOfGold = 35;
        break;
    case 2:
        amountOfItems = 7;
        chanceOfStone = 40;
        chanceOfGold = 40;
        break;
    case 3:
        amountOfItems = 9;
        chanceOfStone = 35;
        chanceOfGold = 35;
        break;
    }

    sizeOfItemArray = amountOfItems;

    Item *items = new Item[amountOfItems];
    for (int i = 0; i < amountOfItems; i++)
    {
        //randomSeed(20);
        int randomType = random(0, 100); // random number between 1 and 100
        enum ItemType type;
        int size;
        int y;
        if (randomType < chanceOfStone) 
        {
            type = STONE;
            size = random(15, 50);
            y = random(110, 180); // stone spawns the highest
        }
        else if (randomType < chanceOfStone + chanceOfGold)
        {
            type = GOLD;
            size = random(15, 50);
            y = random(130, 200); // gold spawns lower than stone
        }
        else
        {
            type = DIAMOND;
            size = 5;
            y = random(200, 230); // diamond can only be on lowest level
        }
        int x = random(0, 320 - size); // random x position witin screen

        Item item(type, x, y, size);
        if (checkCollision(item, items, i))
        {
            i--; // retry creating item
        }
        else
        {
            items[i] = item; // add item to array
        }
    }
    return items;
}