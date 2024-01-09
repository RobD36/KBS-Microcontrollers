#include <util/delay.h>
#include <Wire.h>
#include <IRLib.h>
#include <Fonts/FreeSerifItalic9pt7b.h>
#include "EEPROM.h"
#include "display.h"
#include "items.h"
#include "gamelogic.h"
#include "time.h"
#include "highscore.h"
#include "sevensegment.h"
#include "brightness.h"
#include "Shared.h"
#include "generateItems.h"
#include "buzzer.h"


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
    HIGHSCORES,
    INTERMEDIATE
};
enum menu menuOption = START;

volatile bool firstFrame = true;
volatile bool startMenuPos = true;
volatile bool startGame = false;
volatile bool highscores = false;
volatile bool highscorePos = true;

volatile long milliSeconds;
volatile long startTime;

bool justChangedZ = false;
// int highscoreArray[5] = {3039, 2300, 306, 0, 0};
int *highscoreArray;

//7-segment display
int segmentValue ; //4 = off

display d;
gamelogic g;
time t;
highscore hs;
buzzer b;
sevensegment ss;
brightness br;

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
ISR(TIMER1_COMPA_vect)
{
    t.addTick();
}


//================================================
// Main

int main(void)
{

    d.init();

    b.playStart();

    // use Serial for printing nunchuk data
    // Serial.begin(BAUDRATE);

    // join I2C bus as master
    Wire.begin();

    // Enable global interrupts
    sei();

    while (1)
    {
        milliSeconds = t.getMillisecond();

        Nunchuk.getState(NUNCHUK_ADDRESS);

        b.soundTick(t.getticks());

        ss.clear();

        br.setBrightness(b.getPotentiometerValue());

        if(menuOption == START)

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

            if (Nunchuk.state.z_button == 1 && startMenuPos == true && !justChangedZ)
            {
                menuOption = GAME;
                firstFrame = true;
            }
            else if (Nunchuk.state.z_button == 1 && startMenuPos == false)
            {
                menuOption = HIGHSCORES;
                firstFrame = true;
            }
            justChangedZ = false;
        }

        if (menuOption == GAME)
        {
            ss.printNumber(currentLevel);
            if (firstFrame)
            {
                items = generateItems(t.getticks()); // generate items with time for random seed

                d.fillscreen();
                d.displayLevel();
                startTimeRound = t.getSecond();

                delete[] items;
                g.resetVariables();

                firstFrame = false;

                segmentValue = 1;
                //For debugging until we are actually able to end a game
                //hs.saveHighscore(1200);
            }
            else
            {   //if time is up, go to intermediate screen
                if (g.checkEndOfRound(t.getSecond(), startTimeRound)){
                    menuOption = INTERMEDIATE;
                    firstFrame = true;
                    startTime = milliSeconds;
                }

                gamelogicArray = g.gameTick(items, t.getMillisecond(), t.getSecond());
                d.drawDisplay(gamelogicArray, items, t.getMillisecond(), t.getSecond());
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
                justChangedZ = true;
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

        if(menuOption == INTERMEDIATE) {
            // intermediate screen
            if(firstFrame) {
                d.intermediateScreen();
                currentLevel++;
                firstFrame = false;
            }
            if(milliSeconds - startTime > 5000) {
                if(currentLevel == 4) {
                    currentLevel = 1;
                    menuOption = START;
                    firstFrame = true;
                }
                else {
                    menuOption = GAME;
                    firstFrame = true;
                }
            }
        }
    }

    return 0;
}


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
