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
    tft.print("You: $");
    tft.print(score);
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

void display::generateItems(Item items[])
{
    for(int i = 0; i < 6; i++) {
        Item item = items[i];
        if (item.type == GOLD){
            tft.fillRect(item.x, item.y, item.size, item.size, COLOR_GOLD);
        }
        else if(item.type == STONE) {
            tft.fillRect(item.x, item.y, item.size, item.size, COLOR_ROCK);
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
    tft.fillRect(xBegin - (size/2), yBegin - (size/2), size, size, COLOR_BROWN);
 }

void display::drawHook(int xBegin, int yBegin, int xEnd, int yEnd)
{
    tft.drawLine(xBegin, yBegin, xEnd, yEnd, ILI9341_BLACK);
}

void display::drawItemWhenGrabbed(int xBegin, int yBegin, int size, ItemType type) {
    if(type == GOLD) {
        tft.fillRect(xBegin - (size/2), yBegin - (size/2), size, size, COLOR_GOLD);
    }
    else if(type == STONE) {
        tft.fillRect(xBegin - (size/2), yBegin - (size/2), size, size, COLOR_ROCK);
    }
}

void display::removeItem(int xBegin, int yBegin, int size) {
    tft.fillRect(xBegin, yBegin, size, size, COLOR_BROWN);
}

void display::updateScore(int valueItem) {
    score += valueItem; // Update the score
    tft.fillRect(250, 5, 100, 10, COLOR_BACKGROUND); // Clear previous score
    tft.setCursor(250, 5);
    tft.print("You: $");
    tft.print(String(score));
}

void display::displayItemValue(int valueItem) {
    tft.setCursor(135, 10);
    tft.setTextColor(COLOR_MONEY);
    tft.setFont(&FreeSerifBoldItalic9pt7b);
    tft.print("$");
    tft.print(String(valueItem));
    tft.setTextColor(ILI9341_BLACK);
    tft.setFont(NULL);
    _delay_ms(250);
    // tft.fillRect(134, 0, 40, 20, COLOR_BACKGROUND);
    for(int i = 0; i < 20; i++) {
        tft.fillRect(134, 0, 40, i, COLOR_BACKGROUND);
        _delay_ms(5);
    }
}
