#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <Arduino.h>
#include <Nunchuk.h>
#include "display.h"
#include "items.h"
#include "Shared.h"

#define NUNCHUK_ADDRESS 0x52

// display c;

class gamelogic {
    public:
        gamelogic();

        int* gameTick(Item items[], long ms, long s);

        void saveGamelogicData();

        void getSeconds(long time);
        void getMilliseconds(long time);

        void hookLogic(Item items[]);
        void throwHook(Item items[]);

        void withdrawHookWithItem(Item items[]);

        void withdrawHookWithoutItem();

        void throwHookDown(Item items[]);

        void swingHook();
        void moveCharacter();
        void updateScore();

        bool isEndOfRound(int seconds, int startTimeRound);
        void resetVariables();

    private:
        // int sizeOfArray;

        int returnInformation[18];

        long seconds;
        long milliSeconds;

        long startTime = 0;

        // character
        int characterPositionX;
        int characterPositionY = 55;
        bool characterMovable = true;
        int resetSkySide;
        bool redrawCharacter = true;
        bool drawCharacterFirstTime = true;

        // hook
        uint8_t xOrigin;
        uint8_t yOrigin;

        uint8_t radius = 15;
        float angleStep = 0.05;
        float angle = 0;

        int xBeginHook;
        int yBeginHook = 81;
        int xEndHook;
        int yEndHook;

        int hookCounterSteps = 15;
        int removeHookCounterSteps = 0;
        int stepsTaken;
        int xBeginRemoveHook;
        int yBeginRemoveHook;
        int xEndRemoveHook;
        int yEndRemoveHook;

        bool throwDirectionDown = true;
        bool hookSwinging;
        bool swingDirection = true;
        bool deleteHook = false;
        bool throwingHook = false;
        bool withdrawingHook = false;

        bool itemGrabbedBool = false;
        int itemGrabbed;

        uint8_t xCircle;
        uint8_t yCircle;

        bool turnAround;

        // items
        Item *currentGrabbedItem = nullptr;
        bool scoreHasChanged = false;
        int itemValue = 0;

        bool justChangedC;
        bool justChangedZ;

};


#endif