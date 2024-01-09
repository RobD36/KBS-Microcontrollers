#include "generateItems.h"

// check if item collides with other items
bool checkCollision(Item item, Item itemsArray[], int amountOfItems)
{
    for (int i = 0; i < amountOfItems; i++)
    {
        if (item.type == RAT)
        {
            if (item.x < itemsArray[i].x + itemsArray[i].size + 25 &&
                item.x + item.size + 25 > itemsArray[i].x &&
                item.y < itemsArray[i].y + itemsArray[i].size + 25 &&
                item.y + item.size + 25 > itemsArray[i].y)
            {
                return true;
            }
        }
        else
        {
            if (item.x < itemsArray[i].x + itemsArray[i].size + 3 &&
                item.x + item.size + 3 > itemsArray[i].x &&
                item.y < itemsArray[i].y + itemsArray[i].size + 3 &&
                item.y + item.size + 3 > itemsArray[i].y)
            {
                return true;
            }
        }
    }
    return false;
}

// generate items based on current level
Item *generateItems(long time)
{
    int amountOfItems;
    int chanceOfStone;
    int chanceOfGold;
    int amountOfRats = 0;

    switch (currentLevel)
    {
    case 1:
        amountOfItems = 6; // 5 items on screen
        chanceOfStone = 50;
        chanceOfGold = 35;
        amountOfRats = 1;
        break;
    case 2:
        amountOfItems = 8; // 7 items on screen
        chanceOfStone = 40;
        chanceOfGold = 40;
        amountOfRats = 1;
        break;
    case 3:
        amountOfItems = 10; // 9 items on screen
        chanceOfStone = 35;
        chanceOfGold = 35;
        amountOfRats = 1;
        break;
    }

    sizeOfItemArray = amountOfItems + amountOfRats;

    srand(time);

    Item *items = new Item[amountOfItems + amountOfRats];
    for (int i = 0; i < amountOfItems; i++)
    {
        int randomType = rand() % 100; // random number between 0 and 99
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

        Item item(type, x, y, size); // create item

        // check if item collides with other items
        if (checkCollision(item, items, i))
        {
            i--; // retry creating item
        }
        else
        {
            items[i] = item; // add item to array
        }
    }

    for (int i = 0; i < amountOfRats; i++)
    {
        int x = random(0, 320 - 50);
        int y = random(110, 180);
        int direction = random(0, 2);
        Item item(RAT, x, y, direction);

        if (checkCollision(item, items, amountOfItems + i))
        {
            i--;
        }
        else
        {
            items[amountOfItems + i] = item;
        }
    }

    return items;
}