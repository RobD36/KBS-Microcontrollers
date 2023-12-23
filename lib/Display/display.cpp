#include "display.h"
#include "items.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(10, 9, -1);

display::display() {}


void display::drawDisplay(int returnInformation[], Item items[], int sizeOfArray, long ms, long s)
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

    if (returnInformation[WITHDRAW_HOOK])
    { // remove hook
        removeHook(returnInformation[X_BEGIN_REMOVE_HOOK],
                   returnInformation[Y_BEGIN_REMOVE_HOOK],
                   returnInformation[X_END_REMOVE_HOOK],
                   returnInformation[Y_END_REMOVE_HOOK]);
    }

    generateItems(items, sizeOfArray); // generate items

    if (returnInformation[ITEM_GRABBED_BOOL])
    { // item grabbed
        resetGrabbedItemLocation(items, returnInformation);

        if (returnInformation[WITHDRAW_HOOK])
        {
            resetTrailGrabbedItem(returnInformation, items);
        }
    }

    // check if score has changed
    if (returnInformation[SCORE_HAS_CHANGED])
    {
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
    tft.fillRect(returnInformation[X_END_REMOVE_HOOK] - (items[returnInformation[ITEM_GRABBED_ID]].size / 2),
                 returnInformation[Y_END_REMOVE_HOOK] - (items[returnInformation[ITEM_GRABBED_ID]].size / 2),
                 items[returnInformation[ITEM_GRABBED_ID]].size, items[returnInformation[ITEM_GRABBED_ID]].size,
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

void display::generateItems(Item items[], int sizeOfArray)
{
    for (int i = 0; i < sizeOfArray; i++)
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
    Serial.print(roundDuration);
    Serial.println(" : roundDuration");
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

    tft.fillRect(52, 32, 26, 26, COLOR_GOLD);
    tft.drawRect(50, 30, 30, 30, ILI9341_ORANGE);
    tft.drawRect(51, 31, 28, 28, ILI9341_ORANGE);

    tft.fillTriangle(51, 60, 80, 90, 51, 120, COLOR_ROCK);
    tft.drawTriangle(51, 60, 80, 90, 51, 120, COLOR_WHEELS);
    tft.drawTriangle(52, 61, 80, 89, 52, 119, COLOR_WHEELS);

    tft.fillTriangle(100, 70, 80, 100, 120, 100, COLOR_GOLD);
    tft.drawTriangle(100, 70, 80, 100, 120, 100, ILI9341_ORANGE);
    tft.drawTriangle(99, 69, 81, 100, 119, 99, ILI9341_ORANGE);

    tft.fillRect(240, 50, 30, 30, COLOR_GOLD);
    tft.drawRect(240, 50, 30, 30, ILI9341_ORANGE);
    tft.drawRect(241, 51, 28, 28, ILI9341_ORANGE);

    tft.fillTriangle(240, 30, 269, 30, 269, 60, COLOR_ROCK);
    tft.drawTriangle(240, 30, 269, 30, 269, 60, COLOR_WHEELS);
    tft.drawTriangle(241, 31, 268, 31, 268, 59, COLOR_WHEELS);

    tft.fillRect(230, 85, 30, 30, COLOR_ROCK);
    tft.drawRect(230, 85, 30, 30, COLOR_WHEELS);

    tft.fillRect(250, 70, 30, 30, COLOR_GOLD);
    tft.drawRect(250, 70, 30, 30, ILI9341_ORANGE);
    tft.drawRect(251, 71, 28, 28, ILI9341_ORANGE);

    tft.drawRect(50, 30, 220, 70, ILI9341_BLACK);
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

void display::highscoreCursor(bool cursorPosition)
{
    if (cursorPosition)
    {
        // Reset highscores
        tft.drawRect(5, 197, 60, 20, COLOR_BACKGROUND);
        tft.drawRect(5, 217, 200, 20, ILI9341_BLACK);
    }
    else
    {
        // Back
        tft.drawRect(5, 217, 200, 20, COLOR_BACKGROUND);
        tft.drawRect(5, 197, 60, 20, ILI9341_BLACK);
    }
}