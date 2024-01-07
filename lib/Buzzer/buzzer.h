#ifndef BUZZER_H
#define BUZZER_H

class buzzer
{
public:
    buzzer();
    void test();
    void soundTick(int time);
    void playLader();
    void playStart();

private:
    void timer2_init();
    void moveBuffer();
    void clearBuffer();
};

#endif