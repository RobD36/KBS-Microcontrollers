#include "time.h"
#include "Arduino.h"

time::time()
{
    initateTimer1();
    this->ticks = 0;
}

void time::initateTimer1()
{
    // mode of operation
    TCCR1A |= (1 << WGM21) | (0 << WGM20);
    TCCR1B |= (0 << WGM22);
    // prescaler
    TCCR1B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
    TIMSK1 |= (1 << OCIE2A);
    //set interupt
    OCR1A = 7;
    //set initial value timer
    TCNT1 = 0;
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