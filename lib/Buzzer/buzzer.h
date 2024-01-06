#ifndef BUZZER_H
#define BUZZER_H

class buzzer
{
public:
    buzzer();
    void test();
    void buzzer::soundTick(int time);

private:
    void timer2_init();
    void moveBuffer();
};

#endif