#ifndef DISPLAY_H
#define DISPLAY_H

#include "items.h"

#include <Nunchuk.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>
#include "Shared.h"

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
    void displayScore();
    void displayItemValue(int valueItem);
    void displayStartMenu();
    void menuLogo();
    void startMenuCursor(bool cursor);
    void displayLevel();

    void drawDisplay(int returnInformation[], Item items[], int sizeOfArray);

    void getSeconds(long time);
    void getMilliseconds(long time);

private:
    long startTime = 0;
    long seconds;
    long milliSeconds;

    int currentValueStep = 0;

};

#endif