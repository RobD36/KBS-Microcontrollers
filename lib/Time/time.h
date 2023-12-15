#ifndef TIME_H
#define TIME_H

class time
{
public:
    time();
    long getticks();
    long getSecond();
    long getMilisecond();
    void addTick();

private:
    void initateTimer2();
    long ticks;
};

#endif