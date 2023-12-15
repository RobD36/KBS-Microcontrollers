#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <Arduino.h>
#include <Nunchuk.h>
#include "display.h"

#define NUNCHUK_ADDRESS 0x52

display c;

class gamelogic {
    public:
        gamelogic();

        void gameTick(Item items[]);
        
        void getSeconds(long time);
        void getMilliseconds(long time);

        void drawHook();
        void calculateAndDrawHook();
        void removeHook();
        void moveCharacter();
    
    private:
        uint8_t informationArray[10];

        long seconds;
        long milliSeconds;
        // character
        uint8_t characterPositionX;
        uint8_t characterPositionY;
        bool characterMovable;

        // hook
        uint8_t xOrigin;
        uint8_t yOrigin;

        uint8_t radius;
        float angleStep;
        float angle;

        uint8_t xBeginHook;
        uint8_t yBeginHook;
        uint8_t xEndHook;
        uint8_t yEndHook;

        uint8_t xCircle;
        uint8_t yCircle;

        bool turnAround;

        // items
        Item items[];


        bool justChanged;




};


#endif