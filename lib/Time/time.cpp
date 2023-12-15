#include "time.h"
#include "Arduino.h"

time::time()
{
    initateTimer2();
    this->ticks = 0;
}

void time::initateTimer2()
{
    TCCR2A |= (1 << WGM21) | (0 << WGM20);
    TCCR2B |= (0 << WGM22) | (1 << CS22) | (1 << CS21) | (1 << CS20);
    TIMSK2 |= (1 << OCIE2A);
    OCR2A = 7;
    TCNT2 = 0;

    DDRB |= (1 << PINB5);
    sei();
}

long time::getSecond()
{
    return (ticks / 2000);
}

long time::getMilisecond()
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