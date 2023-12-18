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

        void hookLogic(Item items[]);
        void throwHook(Item items[]);
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
        int hookCounterSteps = 15;
        int removeHookCounterSteps = 0;
        int stepsTaken;
        int xBeginRemoveHook;
        int yBeginRemoveHook;
        int xEndRemoveHook;
        int yEndRemoveHook;

        bool throwDirectionDown = true;
        bool hookSwinging = true; 
        bool swingDirection = true;
        bool deleteHook = false;
        bool throwHookBool = false;
        bool withdrawHook = false; // withdrawHook



        uint8_t xCircle;
        uint8_t yCircle;

        bool turnAround;

        // items


        bool justChangedC;
        bool justChangedZ;




};


#endif