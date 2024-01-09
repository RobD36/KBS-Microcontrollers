#ifndef SEVENSEGMENT_H
#define SEVENSEGMENT_H

#include <Arduino.h>
#include "Wire.h"

class sevensegment
{
public:
    sevensegment();
    void printNumber(int number);
    void clear();

private:
    void display(byte digit);
};
#endif