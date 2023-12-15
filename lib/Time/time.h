#ifndef TIME_H
#define TIME_H

class time
{
public:
    time();
    long getSecond();
    long getMilisecond();
    long ticks;

private:
    void initateTimer2();
};

#endif