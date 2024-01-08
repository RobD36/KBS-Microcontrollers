#include "sevensegment.h"

#define PCF8574A_ADDRESS 0x21

sevensegment::sevensegment() { Wire.begin(); }

void sevensegment::printNumber(int number)
{
    byte digit;

    switch (number)
    {
    case 0:
        digit = 0b11000000;
        break;
    case 1:
        digit = 0b11111001;
        break;
    case 2:
        digit = 0b10100100;
        break;
    case 3:
        digit = 0b10110000;
        break;
    case 4: // Off
        digit = 0b11111111;
        break;
    default:
        digit = 0b11111111;
        break;
    }

    display(digit);
}

void sevensegment::display(byte digit)
{
    Wire.beginTransmission(PCF8574A_ADDRESS);
    Wire.write(digit);
    Wire.endTransmission();
}

void sevensegment::clear(){
    display(0b00000000);
}