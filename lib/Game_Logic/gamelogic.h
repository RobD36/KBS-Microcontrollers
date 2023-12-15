#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <Arduino.h>
#include <Nunchuk.h>
#include "display.h"

#define NUNCHUK_ADDRESS 0x52

// display c;

class gamelogic {
    public:
        gamelogic();

        int* gameTick(Item items[]);
        
        void getSeconds(long time);
        void getMilliseconds(long time);

        void drawHook();
        void calculateAndDrawHook();
        void swingHook();
        void moveCharacter();
    
    private:
        int returnInformation[10];

        long seconds;
        long milliSeconds;

        long startTime = 0;

        // character
        int characterPositionX;
        int characterPositionY = 55;
        bool characterMovable = true;
        int resetSkySide;


        // hook
        uint8_t xOrigin;
        uint8_t yOrigin;

        uint8_t radius = 15;
        float angleStep = 0.05;
        float angle = 0;

        int xBeginHook;
        int yBeginHook;
        int xEndHook;
        int yEndHook;
        int drawOrRemoveHook;

        bool drawHookBool = true;


        uint8_t xCircle;
        uint8_t yCircle;

        bool turnAround;

        // items


        bool justChanged;




};


#endif