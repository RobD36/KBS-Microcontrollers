#ifndef DISPLAY_H
#define DISPLAY_H

#include "items.h"
#include "highscore.h"

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

    void getSeconds(long time);
    void getMilliseconds(long time);

    void init();
    void fillscreen();
    void generateItems(Item items[], int sizeOfArray);
    void drawItemWhenGrabbed(int xBegin, int yBegin, int size, ItemType type);
    void character(int x, int y);
    void resetSkyRight(int xLocation);
    void resetSkyLeft(int xLocation);
    void drawHook(int xBegin, int yBegin, int xEnd, int yEnd);
    void removeHook(int xBegin, int yBegin, int xEnd, int yEnd);
    void removeHookSquare(int xBegin, int yBegin, int size);
    void removeItem(int xBegin, int yBegin, int size);
    void score();
    void itemValue(int valueItem);
    void startMenu();
    void menuLogo();
    void startMenuCursor(bool cursor);
    void displayLevel();
    void highscores();
    void highscoreCursor(bool cursor);

    void drawDisplay(int returnInformation[], Item items[], int sizeOfArray, long ms, long s);

    void fadeItemValue();

private:
    long startTime = 0;
    long seconds;
    long milliSeconds;

    int fadeSteps = 0;

};

#endif