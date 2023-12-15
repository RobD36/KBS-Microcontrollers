#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <Arduino.h>

class gamelogic {
    public:
        gamelogic();
        
        void getTime(uint16_t time);
    
    private:
        uint8_t informationArray[10];

        uint8_t timePassed;

        uint8_t characterPosition;

        uint8_t xBeginHook;
        uint8_t yBeginHook;
        uint8_t xEndHook;
        uint8_t yEndHook;




};


#endif