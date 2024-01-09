#include "display.h"
#include "items.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(10, 9, -1);

display::display() {}

void display::drawDisplay(int returnInformation[], Item items[], long ms, long s)
{
    milliSeconds = ms;
    seconds = s;

    time();

    if (returnInformation[RESET_SKY_SIDE] == 1 && returnInformation[REDRAW_CHARACTER])
    { // reset sky
        resetSkyLeft(returnInformation[CHARACTER_POSITION_X]);
    }
    else if (returnInformation[RESET_SKY_SIDE] == 0 && returnInformation[REDRAW_CHARACTER])
    {
        resetSkyRight(returnInformation[CHARACTER_POSITION_X]);
    }

    if (returnInformation[REDRAW_CHARACTER])
    {
        // character display
        character(returnInformation[CHARACTER_POSITION_X], returnInformation[CHARACTER_POSITION_Y]);
    }

    if (returnInformation[IS_HOOK_SWINGING])
    {
        tft.fillRect(returnInformation[CHARACTER_POSITION_X], 81, 60, 15, COLOR_BROWN);
        drawHook(returnInformation[X_BEGIN_HOOK],
                 returnInformation[Y_BEGIN_HOOK],
                 returnInformation[X_END_HOOK],
                 returnInformation[Y_END_HOOK]);
    }

    if (returnInformation[DELETE_HOOK])
    {
        tft.fillRect(0, 81, 320, 15, COLOR_BROWN);
    }

    if (returnInformation[WITHDRAW_HOOK] && !returnInformation[ITEM_GRABBED_BOOL])
    { // remove hook
        removeHook(returnInformation[X_BEGIN_REMOVE_HOOK],
                   returnInformation[Y_BEGIN_REMOVE_HOOK],
                   returnInformation[X_END_REMOVE_HOOK],
                   returnInformation[Y_END_REMOVE_HOOK]);
    }

    generateItems(items); // generate items

    if (returnInformation[ITEM_GRABBED_BOOL])
    { // item grabbed
        if (returnInformation[WITHDRAW_HOOK])
        {
            resetTrailGrabbedItem(returnInformation, items);
            tempItem = items[returnInformation[ITEM_GRABBED_ID]];
        }
        else
        {
            resetGrabbedItemLocation(items, returnInformation);
        }
    }

    // check if score has changed
    if (returnInformation[SCORE_HAS_CHANGED])
    {
        resetGrabbedItemLocationPulledIn();
        // show value popup
        itemValue(returnInformation[ITEM_VALUE]);
        // display total score
        score();
    }

    if (displayItemValueBool)
    {
        fadeItemValue();
    }
}

void display::resetTrailGrabbedItem(int returnInformation[], Item items[])
{
    // reset trail behind grabbed item
    tft.drawRect(items[returnInformation[ITEM_GRABBED_ID]].x - 1,
                 items[returnInformation[ITEM_GRABBED_ID]].y - 1,
                 items[returnInformation[ITEM_GRABBED_ID]].size + 2,
                 items[returnInformation[ITEM_GRABBED_ID]].size + 2,
                 COLOR_BROWN);

    tft.drawRect(items[returnInformation[ITEM_GRABBED_ID]].x - 2,
                 items[returnInformation[ITEM_GRABBED_ID]].y - 2,
                 items[returnInformation[ITEM_GRABBED_ID]].size + 4,
                 items[returnInformation[ITEM_GRABBED_ID]].size + 4,
                 COLOR_BROWN);
}

void display::resetGrabbedItemLocation(Item items[], int returnInformation[])
{
    // reset grabbed item original location
    tft.fillRect(items[returnInformation[ITEM_GRABBED_ID]].x,
                 items[returnInformation[ITEM_GRABBED_ID]].y,
                 items[returnInformation[ITEM_GRABBED_ID]].size,
                 items[returnInformation[ITEM_GRABBED_ID]].size,
                 COLOR_BROWN);
}

void display::resetGrabbedItemLocationPulledIn()
{
    // reset grabbed item original location
    tft.fillRect(tempItem.x,
                 tempItem.y,
                 tempItem.size,
                 tempItem.size,
                 COLOR_BROWN);
}

void display::init()
{
    // Initialisatie van het LCD-scherm
    tft.begin();
    tft.setRotation(1); // Pas dit aan afhankelijk van de oriëntatie van het scherm

    // Voorbeeld: Tekst "Hello, World!" weergeven op het scherm
    tft.fillScreen(COLOR_BACKGROUND);
}

void display::fillscreen()
{
    tft.fillScreen(COLOR_BACKGROUND);
}

void display::character(int x, int y)
{
    tft.fillCircle(x + 25, y - 20, 10, COLOR_SKIN); // face
    tft.drawPixel(x + 22, y - 23, ILI9341_BLACK);   // eyes
    tft.drawPixel(x + 28, y - 23, ILI9341_BLACK);
    tft.drawFastVLine(x + 26, y - 32, 3, ILI9341_BLACK); // hair
    tft.drawFastVLine(x + 22, y - 32, 3, ILI9341_BLACK);
    tft.drawFastVLine(x + 28, y - 32, 3, ILI9341_BLACK);
    tft.fillRect(x + 5, y - 10, 40, 10, COLOR_BODY); // shirt
    tft.fillRect(x + 10, y - 13, 30, 3, COLOR_BODY);
    tft.fillRect(x, y, 50, 20, COLOR_MINECART); // minecart
    tft.fillRect(x + 5, y + 5, 40, 10, COLOR_BROWN);
    tft.fillRect(x + 5, y + 9, 40, 2, COLOR_MINECART);
    tft.fillCircle(x + 10, y + 20, 4, COLOR_WHEELS); // wheels
    tft.fillCircle(x + 40, y + 20, 4, COLOR_WHEELS);
    tft.fillCircle(x + 10, y, 5, COLOR_SKIN); // hands
    tft.fillCircle(x + 40, y, 5, COLOR_SKIN);
}

void display::resetSkyRight(int xLocation)
{
    tft.fillRect(xLocation + 50, 23, 10, 57, COLOR_BACKGROUND); // right side
    tft.fillRect(xLocation + 30, 23, 25, 37, COLOR_BACKGROUND); // shoulder and face
    tft.fillRect(xLocation, 75, 50, 5, COLOR_BACKGROUND);       // underneath minecart
}

void display::resetSkyLeft(int xLocation)
{
    tft.fillRect(xLocation - 10, 23, 10, 57, COLOR_BACKGROUND); // left side
    tft.fillRect(xLocation, 23, 25, 37, COLOR_BACKGROUND);      // shoulder and face
    tft.fillRect(xLocation, 75, 50, 5, COLOR_BACKGROUND);       // underneath minecart
}

void display::generateItems(Item items[])
{

    for (int i = 0; i < sizeOfItemArray; i++)
    {
        Item item = items[i];
        switch (item.type)
        {
        case GOLD:
            tft.fillRect(item.x, item.y, item.size, item.size, COLOR_GOLD);
            break;
        case STONE:
            tft.fillRect(item.x, item.y, item.size, item.size, COLOR_ROCK);
            break;
        case DIAMOND:
            tft.fillRect(item.x, item.y, item.size, item.size, COLOR_DIAMOND);
        }
    }
};

void display::removeHook(int xBegin, int yBegin, int xEnd, int yEnd)
{
    tft.drawLine(xBegin, yBegin, xEnd, yEnd, COLOR_BROWN);
    tft.drawLine(xBegin + 1, yBegin + 1, xEnd + 1, yEnd + 1, COLOR_BROWN);
    tft.drawLine(xBegin - 1, yBegin - 1, xEnd - 1, yEnd - 1, COLOR_BROWN);
}

void display::removeHookSquare(int xBegin, int yBegin, int size)
{
    tft.fillRect(xBegin - (size / 2), yBegin - (size / 2), size, size, COLOR_BROWN);
}

void display::drawHook(int xBegin, int yBegin, int xEnd, int yEnd)
{
    tft.drawLine(xBegin, yBegin, xEnd, yEnd, ILI9341_BLACK);
}

void display::drawItemWhenGrabbed(int xBegin, int yBegin, int size, ItemType type)
{

    if (type == GOLD)
    {
        tft.fillRect(xBegin - (size / 2), yBegin - (size / 2), size, size, COLOR_GOLD);
    }
    else if (type == STONE)
    {
        tft.fillRect(xBegin - (size / 2), yBegin - (size / 2), size, size, COLOR_ROCK);
    }
    else if (type == DIAMOND)
    {
        tft.fillRect(xBegin - (size / 2), yBegin - (size / 2), size, size, COLOR_DIAMOND);
    }
}

void display::removeItem(int xBegin, int yBegin, int size)
{
    tft.fillRect(xBegin, yBegin, size, size, COLOR_BROWN);
}

void display::score()
{
    tft.fillRect(250, 5, 100, 10, COLOR_BACKGROUND); // Clear previous score
    tft.setCursor(250, 5);
    tft.print("You: $");
    tft.print(currentScore);
}

void display::time()
{
    tft.fillRect(5, 5, 100, 10, COLOR_BACKGROUND);
    tft.setCursor(5, 5);
    tft.print("Time: ");
    tft.print(roundDuration - (seconds - startTimeRound));
}

void display::itemValue(int valueItem)

{
    tft.setCursor(135, 10);
    tft.setTextColor(COLOR_MONEY);
    tft.setFont(&FreeSerifBoldItalic9pt7b);
    tft.print("$");
    tft.print(valueItem);
    tft.setTextColor(ILI9341_BLACK);
    tft.setFont(NULL);

    displayItemValueBool = true;
}

void display::startMenu()
{
    // Display Start/Highscores
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setFont(NULL);
    tft.setCursor(60, 140);
    tft.print("Start");
    tft.setCursor(60, 160);
    tft.print("Highscores");

    // Display character
    character(230, 160);
    // Display logo
    menuLogo();
}

void display::menuLogo()
{
    // Display logo
    tft.fillRect(50, 30, 220, 70, COLOR_LOGO_BROWN);

    displayDecorativeRect(50, 30, 30, 30, "Gold");                       // 1
    displayDecorativeTriangle(50, 60, 80, 100, 50, 100, "C3", "Stone");  // 2
    displayDecorativeRect(240, 50, 30, 30, "Gold");                      // 3
    displayDecorativeRect(230, 85, 30, 30, "Stone");                     // 4
    displayDecorativeRect(72, 90, 10, 10, "Diamond");                    // 5
    displayDecorativeTriangle(80, 100, 100, 70, 120, 100, "X2", "Gold"); // 6
    displayDecorativeTriangle(240, 30, 270, 30, 270, 60, "C2", "Stone"); // 7
    displayDecorativeRect(250, 70, 30, 30, "Gold");                      // 8

    tft.drawRect(50, 30, 220, 70, ILI9341_BLACK); // border

    tft.fillRect(0, 100, 300, 20, COLOR_BACKGROUND);
    tft.fillRect(270, 0, 20, 100, COLOR_BACKGROUND);

    // Display text
    tft.setTextSize(2);
    tft.setCursor(65, 67);
    tft.setFont(&FreeSerifBoldItalic9pt7b);
    tft.print("Goudzoekers");
}

void display::startMenuCursor(bool cursorPosition)
{
    if (cursorPosition)
    {
        // Highscores
        tft.drawRect(50, 137, 77, 20, COLOR_BACKGROUND);
        tft.drawRect(50, 157, 140, 20, ILI9341_BLACK);
    }
    else
    {
        // Start
        tft.drawRect(50, 157, 140, 20, COLOR_BACKGROUND);
        tft.drawRect(50, 137, 77, 20, ILI9341_BLACK);
    }
}

void display::displayLevel()
{
    tft.setFont(NULL);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(1);
    tft.setCursor(5, 5);
    tft.print("Time: ");
    tft.setCursor(250, 5);
    tft.print("You: $");
    tft.print(currentScore);
    tft.setCursor(220, 15);
    tft.print("Opponent: $400");
    tft.fillRect(0, 80, 320, 300, COLOR_BROWN);
}

void display::fadeItemValue()
{
    if (milliSeconds - startTime > 10)
    {
        tft.fillRect(134, 0, 40, fadeSteps, COLOR_BACKGROUND);

        fadeSteps++;

        if (fadeSteps == 20)
        {
            fadeSteps = 0;
            displayItemValueBool = false;
        }

        startTime = milliSeconds;
    }
}

void display::highscores()
{
    tft.fillScreen(COLOR_LOGO_BROWN);
    displayHighscoreDecorative();
    int x = 60;
    int y = 60;

    highscore hs;
    hs.sortHighscore();
    int *array = hs.loadHighscore();

    tft.setCursor(60, 40);
    tft.setTextSize(2);
    tft.setFont(NULL);
    tft.print("Highscores:");

    for (int i = 0; i < 5; i++)
    {

        tft.setCursor(x, y);
        tft.setTextSize(2);
        tft.print(i + 1);
        tft.setCursor(x + 8, y);
        tft.print(".");
        tft.setCursor(x + 25, y);
        tft.print(array[i]);
        y += 20;
    }

    tft.setTextSize(2);
    tft.setCursor(10, 200);
    tft.print("back");

    tft.setCursor(10, 220);
    tft.print("reset highscores");
}

void display::displayDecorativeRect(int x, int y, int width, int height, String material)
{
    if (material == "Gold")
    {
        tft.fillRect(x, y, width, height, COLOR_GOLD);
        tft.drawRect(x, y, width, height, ILI9341_ORANGE);
        tft.drawRect(x + 1, y + 1, width - 2, height - 2, ILI9341_ORANGE);
    }
    if (material == "Stone")
    {
        tft.fillRect(x, y, width, height, COLOR_ROCK);
        tft.drawRect(x, y, width, height, COLOR_WHEELS);
    }
    if (material == "Diamond")
    {
        tft.fillRect(x, y, width, height, COLOR_DIAMOND);
        tft.drawRect(x, y, width, height, COLOR_DIAMOND_BORDER);
    }
}
void display::displayDecorativeTriangle(int x1, int y1, int x2, int y2, int x3, int y3, String orientation, String GoldOrStone)
{
    int sX1;
    int sY1;
    int sX2;
    int sY2;
    int sX3;
    int sY3;

    if (orientation == "Y1")
    {
        sX1 = x1 + 1;
        sY1 = y1 + 1;
        sX2 = x2 - 1;
        sY2 = y2;
        sX3 = x3 + 1;
        sY3 = y3 - 1;
    }
    else if (orientation == "X1")
    {
        sX1 = x1 + 1;
        sY1 = y1 + 1;
        sX2 = x2;
        sY2 = y2 - 1;
        sX3 = x3 - 1;
        sY3 = y3 + 1;
    }
    else if (orientation == "Y2")
    {
        sX1 = x1 + 1;
        sY1 = y1;
        sX2 = x2 - 1;
        sY2 = y2 + 1;
        sX3 = x3 - 1;
        sY3 = y3 - 1;
    }
    else if (orientation == "X2")
    {
        sX1 = x1 + 1;
        sY1 = y1 - 1;
        sX2 = x2;
        sY2 = y2 + 1;
        sX3 = x3 - 1;
        sY3 = y3 - 1;
    }
    else if (orientation == "C1")
    {
        sX1 = x1 + 1;
        sY1 = y1 + 1;
        sX2 = x2 - 1;
        sY2 = y2 + 1;
        sX3 = x3 + 1;
        sY3 = y3 - 1;
    }
    else if (orientation == "C2")
    {
        sX1 = x1 + 1;
        sY1 = y1 + 1;
        sX2 = x2 - 1;
        sY2 = y2 + 1;
        sX3 = x3 - 1;
        sY3 = y3 - 1;
    }
    else if (orientation == "C3")
    {
        sX1 = x1 + 1;
        sY1 = y1 + 1;
        sX2 = x2 - 1;
        sY2 = y2 - 1;
        sX3 = x3 + 1;
        sY3 = y3 - 1;
    }
    else if (orientation == "C4")
    {
        sX1 = x1 + 1;
        sY1 = y1 - 1;
        sX2 = x2 - 1;
        sY2 = y2 + 1;
        sX3 = x3 - 1;
        sY3 = y3 - 1;
    }
    else
    {
        sX1 = x1;
        sY1 = y1;
        sX2 = x2;
        sY2 = y2;
        sX3 = x3;
        sY3 = y3;
    }

    if (GoldOrStone == "Gold")
    {
        tft.fillTriangle(x1, y1, x2, y2, x3, y3, COLOR_GOLD);
        tft.drawTriangle(x1, y1, x2, y2, x3, y3, ILI9341_ORANGE);
        tft.drawTriangle(sX1, sY1, sX2, sY2, sX3, sY3, ILI9341_ORANGE);
    }
    else if (GoldOrStone == "Stone")
    {
        tft.fillTriangle(x1, y1, x2, y2, x3, y3, COLOR_ROCK);
        tft.drawTriangle(x1, y1, x2, y2, x3, y3, COLOR_WHEELS);
        tft.drawTriangle(sX1, sY1, sX2, sY2, sX3, sY3, COLOR_WHEELS);
    }
}

void display::displayHighscoreDecorative()
{
    character(200, 100);

    // items left
    displayDecorativeRect(0, 0, 40, 40, "Gold");                    // 1
    displayDecorativeTriangle(20, 0, 45, 50, 70, 0, "X1", "Stone"); // 2
    displayDecorativeRect(0, 30, 30, 30, "Gold");                   // 3
    displayDecorativeRect(0, 25, 10, 10, "Diamond");                // 4
    displayDecorativeTriangle(0, 50, 30, 70, 0, 90, "Y1", "Stone"); // 5
    displayDecorativeRect(0, 85, 10, 10, "Diamond");                // 6

    // items right above
    displayDecorativeTriangle(280, 0, 320, 0, 320, 40, "C2", "Gold");      // 7
    displayDecorativeRect(250, -1, 40, 20, "Stone");                       // 8
    displayDecorativeTriangle(220, 0, 240, 30, 260, 0, "X1", "Gold");      // 9
    displayDecorativeRect(215, 0, 10, 10, "Diamond");                      // 10
    displayDecorativeRect(290, 60, 30, 30, "Gold");                        // 11
    displayDecorativeTriangle(290, 50, 320, 30, 320, 70, "Y2", "Stone");   // 12
    displayDecorativeRect(310, 30, 10, 10, "Diamond");                     // 13
    displayDecorativeTriangle(280, 110, 320, 90, 320, 130, "Y2", "Stone"); // 14
    displayDecorativeRect(300, 80, 20, 20, "Gold");                        // 15

    // items right below
    displayDecorativeRect(290, 160, 30, 30, "Gold");                       // 16
    displayDecorativeRect(310, 155, 10, 10, "Diamond");                    // 17
    displayDecorativeRect(300, 180, 30, 30, "Stone");                      // 18
    displayDecorativeRect(280, 210, 60, 40, "Gold");                       // 19
    displayDecorativeTriangle(250, 240, 270, 210, 290, 240, "X2", "Gold"); // 20
    displayDecorativeRect(285, 230, 10, 10, "Diamond");                    // 21
}

void display::highscoreCursor(bool cursorPosition)
{
    if (cursorPosition)
    {
        // Reset highscores
        tft.drawRect(5, 197, 60, 20, COLOR_LOGO_BROWN);
        tft.drawRect(5, 217, 200, 20, ILI9341_BLACK);
    }
    else
    {
        // Back
        tft.drawRect(5, 217, 200, 20, COLOR_LOGO_BROWN);

        tft.drawRect(5, 197, 60, 20, ILI9341_BLACK);
    }
}

void display::intermediateScreen()
{
    tft.fillRect(0, 0, 320, 240, COLOR_BACKGROUND);
    tft.setCursor(60, 40);
    tft.setTextSize(2);
    tft.setFont(NULL);
    tft.print("Level: ");
    tft.print(currentLevel);
    tft.print(" completed!");
    tft.setCursor(60, 60);
    tft.print("Current score: $");
    tft.print(currentScore);
}