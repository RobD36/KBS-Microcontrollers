#include "time.h"
#include "Arduino.h"

time::time()
{
    initateTimer2();
    this->ticks = 0;
}

void time::initateTimer2()
{
    // mode of operation
    TCCR2A |= (1 << WGM21) | (0 << WGM20);
    TCCR2B |= (0 << WGM22);
    // prescaler
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
    TIMSK2 |= (1 << OCIE2A);
    //set interupt
    OCR2A = 7;
    //set initial value timer
    TCNT2 = 0;
}

long time::getSecond()
{
    return (ticks / 2000);
}

long time::getMillisecond()
{
    return (ticks / 2);
}

long time::getticks()
{
    return (ticks);
}
void time::addTick(){
    ticks++;
}