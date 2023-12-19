#ifndef DISPLAY_H
#define DISPLAY_H

#include "items.h"

#include <Nunchuk.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>

#include <Fonts/FreeSerifBoldItalic9pt7b.h>

class display
{
public:
    display();

    void init();
    void displayFillScreen();
    void generateItems(Item items[], int sizeOfArray);
    void drawItemWhenGrabbed(int xBegin, int yBegin, int size, ItemType type);
    void displayCharacter(int x, int y);
    void resetSkyRight(int xLocation);
    void resetSkyLeft(int xLocation);
    void drawHook(int xBegin, int yBegin, int xEnd, int yEnd);
    void removeHook(int xBegin, int yBegin, int xEnd, int yEnd);
    void removeHookSquare(int xBegin, int yBegin, int size);
    void removeItem(int xBegin, int yBegin, int size);
    void updateScore(int valueItem);
    void displayItemValue(int valueItem);
    void displayStartMenu();
    void menuLogo();
    void startMenuCursor(bool cursor);
    void displayLevel();

    void drawDisplay(int returnInformation[], Item items[], int sizeOfArray);

    int score = 0;
    bool characterMovable;
    bool turnAround;

private:
    int sizeOfArray;
};

#endif