#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H

#include <Arduino.h>

class brightness
{
    public:
        brightness();
        void setBrightness(int value);
        int getPotentiometerValue();
};

#endif