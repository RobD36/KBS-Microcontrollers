#include "time.h"
#include "Arduino.h"

time::time()
{
    initateTimer1();
    this->ticks = 0;
}

void time::initateTimer1()
{
  // Mode of operation: CTC (Clear Timer on Compare Match)
  TCCR1B |= (1 << WGM12);

  // Set prescaler to 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);

  // Enable Timer/Counter1 Compare Match A interrupt
  TIMSK1 |= (1 << OCIE1A);

  // compare vector
  OCR1A = 8; 

  // Set initial value for the timer
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