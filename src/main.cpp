#include <util/delay.h>
#include <Wire.h>
#include <IRLib.h>
#include <Fonts/FreeSerifItalic9pt7b.h>
#include "display.h"
#include "items.h"
#include "hook.h"


#define ARRAY_SIZE 16

// #define IR_LED_PIN 6;

//================================================
// Global variables
// LCD Screen
int xLocation = 0;
int yLocation = 0;
bool characterMovable = true;

bool justChanged = false;


//Startmenu
volatile bool menuPos = false;
volatile bool menuAcceptStart = false;
volatile bool menuAcceptHighscores = false;
volatile bool startDrawn = false;

display d;
hook h;

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

Item gold1(GOLD, 20, 150, 10);
Item gold2(GOLD, 100, 100, 15);
Item gold3(GOLD, 200, 150, 20);
Item gold4(GOLD, 250, 180, 25);

Item stone1(STONE, 10, 200, 10);
Item stone2(STONE, 50, 100, 15);
Item stone3(STONE, 250, 150, 20);
Item stone4(STONE, 200, 180, 25);

Item items[] = {gold1, gold2, gold3, gold4, stone1, stone2, stone3, stone4};

//================================================
// Pre defines of functions
// Nunchuck
bool readNunchuck();

// LCD screen
void drawCircle();
void eraseCircle();
void displayCharacter(int x, int y);
void resetSkyRight(int xLocation);
void resetSkyLeft(int xLocation);
void createBlocks(int Small, int Medium, int big);
void displayStartMenu();
void drawHook(int xLocation);

// IR
void convertArray();

//================================================
// Interrupts
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

    // Initialise IR sensor pin
    DDRD |= (1 << DDD6);
    // Enable external interrupt 0
    EICRA |= (1 << ISC00);
    EIMSK |= (1 << INT0);
    // Initialise timers
    // Timer 1
    TCCR1B = (1 << CS10) | (1 << CS12); // Set prescaler to 1024
    TCNT1 = 0;
    // Timer 2
    TCCR2A |= (1 << WGM21);          // CTC mode
    OCR2A = (F_CPU / 1000000UL) - 1; // Set compare value for 1 microsecond delay
    TIMSK2 |= (1 << OCIE2A);         // Enable compare interrupt


    d.displayCharacter(xLocation, 55);

    // use Serial for printing nunchuk data
    Serial.begin(BAUDRATE);

    // join I2C bus as master
    Wire.begin();

    // Enable global interrupts
    sei();

    d.generateItems(items);

    // Eindeloze lus
    while (1)
    {
        Nunchuk.getState(NUNCHUK_ADDRESS);
        if (Nunchuk.state.c_button == 0 && justChanged)
        {
            justChanged = false;
        }
        /*
        eraseCircle();
        readNunchuck();
        drawCircle();
        */


        // Receive
        // if(fullPulseArray && validBit){
        //  convertArray();
        // }

        // Send
        // sendSignal(testArray, sizeof(testArray));
        //_delay_ms(2000);

        if (!Nunchuk.getState(NUNCHUK_ADDRESS))

            return (false);

       /*
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
        */

        displayStartMenu();


    }

    return 0;
}


void displayStartMenu()
{
    if(!startDrawn){
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(60, 140);
    tft.print("Start");
    tft.setCursor(60, 160);
    tft.print("Highscores");
    displayCharacter(230, 160);
    tft.fillRect(50, 30, 220, 70, COLOR_LOGO_BROWN);

    tft.fillRect(52, 32, 26, 26, COLOR_GOLD);
    tft.drawRect(50, 30, 30, 30, ILI9341_ORANGE);
    tft.drawRect(51, 31, 28, 28, ILI9341_ORANGE);

    tft.fillTriangle(51, 60, 80, 90, 51, 120, COLOR_ROCK);
    tft.drawTriangle(51, 60, 80, 90, 51, 120, COLOR_WHEELS);
    tft.drawTriangle(52, 61, 80, 89, 52, 119, COLOR_WHEELS);

    tft.fillTriangle(100, 70, 80, 100, 120, 100, COLOR_GOLD);
    tft.drawTriangle(100, 70, 80, 100, 120, 100, ILI9341_ORANGE);
    tft.drawTriangle(99, 69, 81, 100, 119, 99, ILI9341_ORANGE);

    tft.fillRect(240, 50, 30, 30, COLOR_GOLD);
    tft.drawRect(240, 50, 30, 30, ILI9341_ORANGE);
    tft.drawRect(241, 51, 28, 28, ILI9341_ORANGE);

    tft.fillTriangle(240, 30, 269, 30, 269, 60, COLOR_ROCK);
    tft.drawTriangle(240, 30, 269, 30, 269, 60, COLOR_WHEELS);
    tft.drawTriangle(241, 31, 268, 31, 268, 59, COLOR_WHEELS);

    tft.fillRect(230, 85, 30, 30, COLOR_ROCK);
    tft.drawRect(230, 85, 30, 30, COLOR_WHEELS);

    tft.fillRect(250, 70, 30, 30, COLOR_GOLD);
    tft.drawRect(250, 70, 30, 30, ILI9341_ORANGE);
    tft.drawRect(251, 71, 28, 28, ILI9341_ORANGE);

    tft.drawRect(50, 30, 220, 70, ILI9341_BLACK);
    tft.fillRect(0,100, 300, 20, COLOR_BACKGROUND);
    tft.fillRect(270, 0, 20, 100, COLOR_BACKGROUND);
    tft.setTextSize(2);
    tft.setCursor(65,67);
    tft.setFont(&FreeSerifItalic9pt7b);
    tft.print("Goudzoekers");
    startDrawn = true;
    }

    if(Nunchuk.state.joy_y_axis < 128)
    {
        menuPos = true;
    }
    else if(Nunchuk.state.joy_y_axis > 128)
    {
        menuPos = false;
    }

    if(menuPos)
    {
        //Highscores
        tft.drawRect(50, 137, 77, 20, COLOR_BACKGROUND);
        tft.drawRect(50, 157, 140, 20, ILI9341_BLACK);
        if(Nunchuk.state.c_button == 1){ menuAcceptHighscores = true; }
    }
    else
    {
        //Start
        tft.drawRect(50, 157, 140, 20, COLOR_BACKGROUND);
        tft.drawRect(50, 137, 77, 20, ILI9341_BLACK);
        if(Nunchuk.state.c_button == 1){ menuAcceptStart = true; }
    }

}




//================================================
// Functions
// Nunchuck
bool readNunchuck()
{
    if (!Nunchuk.getState(NUNCHUK_ADDRESS))
        return (false);
    {

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
        if (intValueY > 128 && yLocation > 0)
        {
            yLocation--;
        }
        if (intValueY < 128 && yLocation < 240)
        {
            yLocation++;
        }
        // _delay_ms(50);

        if ((intValueX > 128 && xLocation < 270) && characterMovable)
        {
            xLocation += 5;
            d.resetSkyLeft(xLocation);
            d.displayCharacter(xLocation, 55);
        }

        if (Nunchuk.state.c_button == 1)
        {
            characterMovable = false;
            drawHook(xLocation);
        }
    }

    return 0;
}

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
