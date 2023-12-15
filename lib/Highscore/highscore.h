#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <Arduino.h>
#include "EEPROM.h"

class highscore
{
    public:
        highscore();

        void saveHighscore(int score);
        int* loadHighscore();
        void sortHighscore();

};

#endif