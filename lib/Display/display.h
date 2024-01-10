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

    void init();
    void fillscreen();
    void generateItems(Item items[]);
    void character(int x, int y);
    void resetSkyRight(int xLocation);
    void resetSkyLeft(int xLocation);
    void drawHook(int xBegin, int yBegin, int xEnd, int yEnd);
    void removeHook(int xBegin, int yBegin, int xEnd, int yEnd);
    void score();
    void time();
    void itemValue(int valueItem);
    void startMenu();
    void menuLogo();
    void startMenuCursor(bool cursor);
    void displayLevel(Item items[]);

    void resetSky(int x, int y);

    void displayHighscoreDecorative();
    void displayDecorativeTriangle(int x1, int y1, int x2, int y2, int x3, int y3, String orientation, String GoldOrStone);
    void displayDecorativeRect(int x, int y, int width, int height, String material);

    void highscores();

    void highscoreCursor(bool cursor);
    void intermediateScreen();

    void drawDisplay(int returnInformation[], Item items[], long ms, long s);

    void resetTrailGrabbedItem(int returnInformation[], Item items[]);

    void resetGrabbedItemLocation(Item items[], int returnInformation[]);
    void resetGrabbedItemLocationPulledIn();

    void fadeItemValue();

    void rat(int x, int y, int direction);
    void moveRats(Item items[]);

private:
    long startTime = 0;
    long seconds;
    long milliSeconds;

    int fadeSteps = 0;
    Item tempItem;
};

#endif