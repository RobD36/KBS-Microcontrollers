#ifndef DISPLAY_H
#define DISPLAY_H

#include "items.h"
#include "highscore.h"

#include <Nunchuk.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>

#include <Fonts/FreeSerifBoldItalic9pt7b.h>

#define NUNCHUK_ADDRESS 0x52
#define WAIT 1000
#define BAUDRATE 9600
#define CHUNKSIZE 32
#define BUFFERLEN 256

class display
{
public:
    display();

    void init();
    void displayFillScreen();
    void generateItems(Item items[]);
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
    void displayHighscore();
    void displayHighscoreDecorative();
    void displayDecorativeTriangle(int x1, String smallerX1, int y1, String smallerY1, int x2, String smallerX2, int y2, String smallerY2, 
                                    int x3, String smallerX3, int y3, String smallerY3, String GoldOfStone);
    void displayDecorativeRect(int x, int y, int width, int height, String material);
    void highscoreCursor(bool cursor);

    int score = 0;
    bool characterMovable;
    bool turnAround;
};

#endif