#include <util/delay.h>
#include <Wire.h>
#include <IRLib.h>
#include <Fonts/FreeSerifItalic9pt7b.h>
#include "EEPROM.h"
#include "display.h"
#include "items.h"
#include "hook.h"
#include "time.h"
#include "highscore.h"
#include "sevensegment.h"

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
enum menu
{
    START,
    GAME,
    HIGHSCORES
};
enum menu menuOption = START;
volatile bool firstFrame = true;
volatile bool startMenuPos = true;
volatile bool startGame = false;
volatile bool highscores = false;
volatile bool highscorePos = true;
//int highscoreArray[5] = {3039, 2300, 306, 0, 0};
int* highscoreArray;

//7-segment display
int segmentValue = 4; //4 = off

display d;
hook h;
time t;
highscore hs;
sevensegment ss;

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

Item gold1(GOLD, 20, 150, 30);
Item gold2(GOLD, 100, 160, 50);
Item gold3(GOLD, 180, 150, 20);

Item stone1(STONE, 10, 200, 15);
Item stone2(STONE, 50, 110, 15);
Item stone3(STONE, 240, 110, 50);

Item diamond1(DIAMOND, 260, 210, 5);
Item diamond2(DIAMOND, 200, 200, 5);
Item diamond3(DIAMOND, 40, 220, 5);

Item items[] = {gold1, gold2, gold3, stone1, stone2, stone3, diamond1, diamond2, diamond3};

//================================================
// Pre defines of functions

// LCD screen
void drawHook(int xLocation);

// IR
void convertArray();

//================================================
// Interrupts
ISR(TIMER2_COMPA_vect){
  t.addTick();
}

ISR(INT0_vect)
{
    if (PIND & (1 << PD2))
    {
        // Debugging
        // receiveOk = true;
        if (TCNT1 > 3000) // Receive start signal
        {
            pulseArrayCounter = 0;
        }
        else
        {
            pulseArray[pulseArrayCounter] = TCNT1; // Signal into pulseArray
            pulseArrayCounter++;
        }
        TCNT1 = 0;
        if (pulseArrayCounter == ARRAY_SIZE)
        {
            fullPulseArray = true;
            isInterrupt = true;
        }
    }
    else
    {
        // Falling edge (Not used)
    }
}

//================================================
// Main

int main(void)
{

    d.init();


    // use Serial for printing nunchuk data
    Serial.begin(BAUDRATE);

    // join I2C bus as master
    Wire.begin();

    // Enable global interrupts
    sei();

    while(1)
    {
        if(!Nunchuk.getState(NUNCHUK_ADDRESS))
            return (false);

        ss.printNumber(segmentValue);

        if(menuOption == START)
        {
            if (firstFrame)
            {
                d.displayFillScreen();
                d.displayStartMenu();
                d.startMenuCursor(false);
                highscoreArray = hs.loadHighscore();

                startMenuPos = true;
                firstFrame = false;
            }

            Nunchuk.getState(NUNCHUK_ADDRESS);
            if (Nunchuk.state.z_button == 0 && justChanged)
            {
                justChanged = false;
            }

            if (Nunchuk.state.z_button == 1 && !justChanged)
            {
                justChanged = true;
            }

            if (Nunchuk.state.joy_y_axis < 128)
            {
                // Highscores
                d.startMenuCursor(true);
                startMenuPos = false;
            }
            else if (Nunchuk.state.joy_y_axis > 128)
            {
                // Start
                d.startMenuCursor(false);
                startMenuPos = true;
            }

            if (Nunchuk.state.z_button == 1 && startMenuPos == true)
            {
                menuOption = GAME;
                firstFrame = true;
            }
            else if (Nunchuk.state.z_button == 1 && startMenuPos == false)
            {
                menuOption = HIGHSCORES;
                firstFrame = true;
            }
        }

        if(menuOption == GAME)
        {
            if(firstFrame)
            {
                d.displayFillScreen();
                d.displayLevel();
                d.displayCharacter(xLocation, 55);
                d.generateItems(items);
                firstFrame = false;
                segmentValue = 1;
                //For debugging until we are actually able to end a game
                //hs.saveHighscore(1200);
            }
            
            Nunchuk.getState(NUNCHUK_ADDRESS);
            if (Nunchuk.state.c_button == 0 && justChanged)
            {
                justChanged = false;
            }

            if (!Nunchuk.getState(NUNCHUK_ADDRESS))

                return (false);

            int intValueX = static_cast<int>(Nunchuk.state.joy_x_axis);
            int intValueY = static_cast<int>(Nunchuk.state.joy_y_axis);

            // move character left and right
            if ((intValueX < 128 && xLocation > 0) && characterMovable)
            {
                xLocation -= 5;
                d.resetSkyRight(xLocation);
                d.displayCharacter(xLocation, 55);
            }
            if ((intValueX > 128 && xLocation < 270) && characterMovable)
            {
                xLocation += 5;
                d.resetSkyLeft(xLocation);
                d.displayCharacter(xLocation, 55);
            }

            if (Nunchuk.state.c_button == 1 && !justChanged)
            {
                justChanged = true;
                characterMovable = false;
                drawHook(xLocation);
            }
        }

        if(menuOption == HIGHSCORES)
        {
            if(firstFrame)
            {
                d.displayFillScreen();
                d.displayHighscore();
                d.highscoreCursor(false);
                highscorePos = true;
                firstFrame = false;
            }

            Nunchuk.getState(NUNCHUK_ADDRESS);
            if (Nunchuk.state.z_button == 0 && justChanged)
            {
                justChanged = false;
            }

            if (Nunchuk.state.z_button == 1 && !justChanged)
            {
                justChanged = true;
            }

            if (Nunchuk.state.joy_y_axis < 128)
            {
                // Highscores
                d.highscoreCursor(true);
                highscorePos = false;
            }
            else if (Nunchuk.state.joy_y_axis > 128)
            {
                // Start
                d.highscoreCursor(false);
                highscorePos = true;
            }

            if (Nunchuk.state.z_button == 1 && highscorePos == true)
            {
                menuOption = START;
                firstFrame = true;
            }
            else if (Nunchuk.state.z_button == 1 && highscorePos == false)
            {
                hs.resetHighscores();
                menuOption = HIGHSCORES;
                firstFrame = true;
            }
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
