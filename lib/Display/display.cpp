#include "display.h"
#include "items.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(10, 9, -1);

display::display() {}

void display::init()
{
    // Initialisatie van het LCD-scherm
    tft.begin();
    tft.setRotation(1); // Pas dit aan afhankelijk van de oriëntatie van het scherm

    // Voorbeeld: Tekst "Hello, World!" weergeven op het scherm
    tft.fillScreen(COLOR_BACKGROUND);
}

void display::displayFillScreen()
{
    tft.fillScreen(COLOR_BACKGROUND);
}

void display::displayCharacter(int x, int y)
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

void display::displayScore()
{
    Serial.println(score);  
    tft.fillRect(250, 5, 100, 10, COLOR_BACKGROUND); // Clear previous score
    tft.setCursor(250, 5);
    tft.print("You: $");
    tft.print(score);
}

void display::displayItemValue(int valueItem)
{
    tft.setCursor(135, 10);
    tft.setTextColor(COLOR_MONEY);
    tft.setFont(&FreeSerifBoldItalic9pt7b);
    tft.print("$");
    tft.print(String(valueItem));
    tft.setTextColor(ILI9341_BLACK);
    tft.setFont(NULL);
    _delay_ms(250);
    // tft.fillRect(134, 0, 40, 20, COLOR_BACKGROUND);
    for (int i = 0; i < 20; i++)
    {
        tft.fillRect(134, 0, 40, i, COLOR_BACKGROUND);
        _delay_ms(5);
    }
}

void display::displayStartMenu()
{
    // Display Start/Highscores
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(60, 140);
    tft.print("Start");
    tft.setCursor(60, 160);
    tft.print("Highscores");

    // Display character
    displayCharacter(230, 160);
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
        // if(Nunchuk.state.c_button == 1){ menuAcceptHighscores = true; }
    }
    else
    {
        // Start
        tft.drawRect(50, 157, 140, 20, COLOR_BACKGROUND);
        tft.drawRect(50, 137, 77, 20, ILI9341_BLACK);
        // if(Nunchuk.state.c_button == 1){ menuAcceptStart = true; }
    }
}

void display::displayLevel()
{
    tft.setFont(NULL);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(1);
    tft.setCursor(5, 5);
    tft.print("Time: 120");
    tft.setCursor(250, 5);
    tft.print("You: $");
    tft.print(score);
    tft.setCursor(220, 15);
    tft.print("Opponent: $400");
    tft.fillRect(0, 80, 320, 300, COLOR_BROWN);
}

void display::drawDisplay(int returnInformation[], Item items[], int sizeOfArray)
{

    if (returnInformation[2] == 1)
    { // reset sky
        resetSkyLeft(returnInformation[0]);
    }
    else if (returnInformation[2] == 0)
    {
        resetSkyRight(returnInformation[0]);
    }

    displayCharacter(returnInformation[0], returnInformation[1]); // character display

    if (!returnInformation[7])
    { // draw hook
    }
    else if (returnInformation[7])
    {
        // removeHook(returnInformation[3], returnInformation[4], returnInformation[5], returnInformation[6]);
        tft.fillRect(returnInformation[0], 81, 60, 15, COLOR_BROWN);
        drawHook(returnInformation[3], returnInformation[4], returnInformation[5], returnInformation[6]);
    }

    if (returnInformation[8])
    {
        tft.fillRect(0, 81, 320, 15, COLOR_BROWN);
    }

    if (returnInformation[9])
    { // remove hook
        removeHook(returnInformation[10], returnInformation[11], returnInformation[12], returnInformation[13]);
    }

    generateItems(items, sizeOfArray); // generate items

    if (returnInformation[14])
    { // item grabbed
        // reset grabbed item original location
        tft.fillRect(items[returnInformation[15]].x, items[returnInformation[15]].y, items[returnInformation[15]].size, items[returnInformation[15]].size, COLOR_BROWN);
        if (returnInformation[9])
        {
            // reset trail behind grabbed item
            tft.fillRect(returnInformation[12] - (items[returnInformation[15]].size / 2), returnInformation[13] - (items[returnInformation[15]].size / 2), items[returnInformation[15]].size, items[returnInformation[15]].size, COLOR_BROWN);
        }
    }

    // check if score has changed
    if (returnInformation[16])
    {
        displayScore();
    }
}