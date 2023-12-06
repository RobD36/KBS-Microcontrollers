#ifndef IRLIB_H
#define IRLIB_H

#include <Arduino.h>

void IRpulse(long microSeconds);
void delayTimer(uint16_t microSeconds);
void sendTestSignal();
void printIntArray(int array[], int size);

#endif //IRLIB_H