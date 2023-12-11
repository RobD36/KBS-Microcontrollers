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

    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(1);
    tft.setCursor(5, 5);
    tft.print("Time: 120");
    tft.setCursor(250, 5);
    tft.print("You: $500");
    tft.setCursor(220, 15);
    tft.print("Opponent: $400");
    tft.fillRect(0, 80, 320, 300, COLOR_BROWN);
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

void display::generateItems()
{
    Item gold1(GOLD, 20, 150, 10);
    Item gold2(GOLD, 100, 100, 15);
    Item gold3(GOLD, 200, 200, 20);

    Item stone1(STONE, 10, 200, 10);
    Item stone2(STONE, 50, 100, 15);
    Item stone3(STONE, 250, 150, 20);

    tft.fillRect(gold1.x, gold1.y, gold1.size, gold1.size, COLOR_GOLD);
    tft.fillRect(gold2.x, gold2.y, gold2.size, gold2.size, COLOR_GOLD);
    tft.fillRect(gold3.x, gold3.y, gold3.size, gold3.size, COLOR_GOLD);

    tft.fillRect(stone1.x, stone1.y, stone1.size, stone1.size, COLOR_ROCK);
    tft.fillRect(stone2.x, stone2.y, stone2.size, stone2.size, COLOR_ROCK);
    tft.fillRect(stone3.x, stone3.y, stone3.size, stone3.size, COLOR_ROCK);
};

void display::removeHook(int xBegin, int yBegin, int xEnd, int yEnd)
{
    tft.drawLine(xBegin, yBegin, xEnd, yEnd, COLOR_BROWN);
    tft.drawLine(xBegin + 1, yBegin + 1, xEnd + 1, yEnd + 1, COLOR_BROWN);
    tft.drawLine(xBegin - 1, yBegin - 1, xEnd - 1, yEnd - 1, COLOR_BROWN);
}

void display::drawHook(int xBegin, int yBegin, int xEnd, int yEnd)
{
    tft.drawLine(xBegin, yBegin, xEnd, yEnd, ILI9341_BLACK);
}

void display::removesquare(int xBegin, int yBegin, int xEnd, int yEnd)
{
    tft.fillRect(xBegin, yBegin, xEnd, yEnd, COLOR_BROWN);
}
