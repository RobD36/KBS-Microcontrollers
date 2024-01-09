#ifndef TIME_H
#define TIME_H

class time
{
public:
    time();
    long getticks();// returns the current ticks
    long getSecond();// returns the current seconds
    long getMillisecond();// returns the current milliseconds
    void addTick(); // this function is used by an interupt on the timer to count ticks

private:
    void initateTimer1();
    long ticks;
};

#endif