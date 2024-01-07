#include <util/delay.h>
#include <Wire.h>
#include <IRLib.h>
#include <Fonts/FreeSerifItalic9pt7b.h>
#include "display.h"
#include "items.h"
#include "time.h"
#include "buzzer.h"
#include "highscore.h"
#include "Shared.h"
#include "generateItems.h"

#define ARRAY_SIZE 16

// #define IR_LED_PIN 6;

//================================================
// Global variables
// LCD Screen
int xLocation = 0;
int yLocation = 0;
bool characterMovable = true;

bool justChanged = false;

// Startmenu
volatile bool menuPos = false;
volatile bool startGame = false;

display d;
hook h;
time t;
buzzer b;

// IR

volatile bool isInterrupt = false;
volatile bool fullPulseArray = false;
volatile bool validBit = false;
volatile bool receiveOk = false;

volatile uint32_t pulseArrayCounter = 0;

int pulseArray[ARRAY_SIZE];
int bitArray[ARRAY_SIZE];
int testArray[ARRAY_SIZE] = {1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0}; // Last bit not used

// Items
Item *items;



int *gamelogicArray;


//================================================
// Pre defines of functions

// LCD screen
void drawHook(int xLocation);

// IR
void convertArray();
void initTimers();

//================================================
// Interrupts
ISR(TIMER1_COMPA_vect)
{
    t.addTick();
}


//================================================
// Main

int main(void)
{

    d.init();

    // set startup sound
    b.playStart();


    // use Serial for printing nunchuk data
    Serial.begin(BAUDRATE);

    // join I2C bus as master
    Wire.begin();
    // Enable global interrupts
    sei();

    d.displayStartMenu();
    d.startMenuCursor(false);

    while (!startGame)
    {
        b.soundTick(t.getticks());
        if (!Nunchuk.getState(NUNCHUK_ADDRESS))

            return (false);

        if (Nunchuk.state.joy_y_axis < 128)
        {
            // Highscores
            d.startMenuCursor(true);
            menuPos = false;
        }
        else if (Nunchuk.state.joy_y_axis > 128)
        {
            // Start
            d.startMenuCursor(false);
            menuPos = true;
        }

        if (Nunchuk.state.c_button == 1 && menuPos == true)
        {
            startGame = true;
        }
        else if (Nunchuk.state.c_button == 1 && menuPos == false)
        {
        }
    }

    d.displayFillScreen();
    d.displayLevel();
    d.displayCharacter(xLocation, 55);
    d.generateItems(items);

    // main loop
    while (1)
    {
        Nunchuk.getState(NUNCHUK_ADDRESS);
        if (Nunchuk.state.c_button == 0 && justChanged)
        {
            justChanged = false;
        }


        if (menuOption == GAME)
        {
            if (firstFrame)
            {   
                sizeOfItemArray = 10;
                items = generateItems(sizeOfItemArray);

                d.fillscreen();
                d.displayLevel();
                startTimeRound = t.getSecond();

                delete[] items;
                g.resetVariables();

                firstFrame = false;
                // For debugging until we are actually able to end a game
                // hs.saveHighscore(1200);
            }
            else
            {   //if time is up, go to start menu
                if (g.checkEndOfRound(t.getSecond(), startTimeRound)){
                    menuOption = START;
                    firstFrame = true;
                }

                gamelogicArray = g.gameTick(items, t.getMillisecond(), t.getSecond());
                d.drawDisplay(gamelogicArray, items, sizeOfItemArray, t.getMillisecond(), t.getSecond());
            }
        }

        if (Nunchuk.state.c_button == 1 && !justChanged)
        {
            justChanged = true;
            characterMovable = false;
            drawHook(xLocation);
        }
    }

    return 0;
}

//================================================
// Functions

// IR
// Convert pulse array to bit array based on pulse lengths

void convertArray()
{
    if (fullPulseArray)
    {
        // For debugging
        printIntArray(pulseArray, (sizeof(pulseArray) / 2));

        for (uint16_t i = 0; i < (sizeof(pulseArray) / 2); i++)
        {
            // Checking for pulse lenghts and set to binary
            if (pulseArray[i] >= 16 && pulseArray[i] <= 20) // Pulse length between 16 & 20 = 0
            {
                bitArray[i] = 0;
            }
            else if (pulseArray[i] >= 33 && pulseArray[i] <= 37) // Pulse length between 33 & 37 = 1
            {
                bitArray[i] = 1;
            }
            pulseArray[i] = 0; // Pulse array reset
        }
        if ((sizeof(bitArray) / 2) == 16)
        {
            validBit = true;
        }
        pulseArrayCounter = 0; // Reset pulse counter

        // For debugging
        printIntArray(bitArray, (sizeof(bitArray) / 2));
    }
}

void drawHook(int xLocation)
{
    int xOrigin = xLocation + 25;
    int yOrigin = 81;

    // Calculate the radius of the circle
    int radius = 15;

    // Calculate the angle step to draw points on the circle
    float angleStep = 0.05; // Change this to adjust the spacing of points on the circle

    while (!characterMovable)
    {
        // swing right to left
        for (float angle = 0; angle < 1 * PI; angle += angleStep)
        {
            Nunchuk.getState(NUNCHUK_ADDRESS);
            if (Nunchuk.state.c_button == 0 && justChanged)
            {
                justChanged = false;
            }

            if (Nunchuk.state.c_button == 1 && !justChanged)
            {
                justChanged = true;
                characterMovable = true;
                break;
            }

            if (Nunchuk.state.z_button == 1)
            {
                h.calculateAndDrawHook(xOrigin, yOrigin, angle, items);
            }
            else
            {
                h.removeHook(xOrigin, yOrigin, radius, angle);
            }
        }

        if (!characterMovable)
        {
            // swing left to right
            for (float angle = PI; angle > 0; angle -= angleStep)
            {
                Nunchuk.getState(NUNCHUK_ADDRESS);
                if (Nunchuk.state.c_button == 0 && justChanged)
                {
                    justChanged = false;
                }

                if (Nunchuk.state.c_button == 1 && !justChanged)
                {
                    justChanged = true;
                    characterMovable = true;
                    break;
                }

                if (Nunchuk.state.z_button == 1)
                {
                    h.calculateAndDrawHook(xOrigin, yOrigin, angle, items);
                }
                else
                {
                    h.removeHook(xOrigin, yOrigin, radius, angle);
                }
            }
        }
    }
}
