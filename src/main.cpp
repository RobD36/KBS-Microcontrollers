#include <util/delay.h>
#include <Wire.h>
#include <IRLib.h>
#include <Fonts/FreeSerifItalic9pt7b.h>
#include "EEPROM.h"
#include "display.h"
#include "items.h"
#include "hook.h"
#include "gamelogic.h"
#include "time.h"
#include "highscore.h"
#include "Shared.h"

#define ARRAY_SIZE 16
#define NUNCHUK_ADDRESS 0x52
#define BAUDRATE 9600

// #define IR_LED_PIN 6;

//================================================
// Global variables

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
// int highscoreArray[5] = {3039, 2300, 306, 0, 0};
int *highscoreArray;

display d;
hook h;
gamelogic g;
time t;
highscore hs;

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

Item gold1(GOLD, 20, 150, 30);  // 0
Item gold2(GOLD, 100, 160, 50); // 1
Item gold3(GOLD, 180, 150, 20); // 2

Item stone1(STONE, 10, 200, 15);  // 3
Item stone2(STONE, 50, 110, 15);  // 4
Item stone3(STONE, 240, 110, 50); // 5

Item diamond1(DIAMOND, 260, 210, 5);
Item diamond2(DIAMOND, 200, 200, 5);
Item diamond3(DIAMOND, 40, 220, 5);

Item items[] = {gold1, gold2, gold3, stone1, stone2, stone3, diamond1, diamond2, diamond3};

int sizeOfItemArray = 9;

int *gamelogicArray;

//================================================
// Pre defines of functions
// Nunchuck
bool readNunchuck();

// LCD screen
void character(int x, int y);
void resetSkyRight(int xLocation);
void resetSkyLeft(int xLocation);
void createBlocks(int Small, int Medium, int big);
void startMenu();
void drawHook(int xLocation);

// IR
void convertArray();

//================================================
// Interrupts
ISR(TIMER2_COMPA_vect)
{
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

    while (1)
    {
        Nunchuk.getState(NUNCHUK_ADDRESS);

        if (menuOption == START)
        {
            if (firstFrame)
            {
                d.fillscreen();
                d.startMenu();
                d.startMenuCursor(false);
                highscoreArray = hs.loadHighscore();

                startMenuPos = true;
                firstFrame = false;
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

        if (menuOption == GAME)
        {
            if (firstFrame)
            {
                d.fillscreen();
                d.displayLevel();

                firstFrame = false;
                // For debugging until we are actually able to end a game
                // hs.saveHighscore(1200);
            }
            else
            {
                gamelogicArray = g.gameTick(items, t.getMillisecond(), t.getSecond());
                d.drawDisplay(gamelogicArray, items, sizeOfItemArray, t.getMillisecond(), t.getSecond());
            }
        }

        if (menuOption == HIGHSCORES)
        {
            if (firstFrame)
            {
                d.fillscreen();
                d.highscores();
                d.highscoreCursor(false);
                highscorePos = true;
                firstFrame = false;
            }

            Nunchuk.getState(NUNCHUK_ADDRESS);

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
