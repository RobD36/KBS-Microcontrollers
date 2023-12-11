#include <util/delay.h>
#include <Wire.h>
#include <IRLib.h>
#include "display.h"
#include "items.h"

#define ARRAY_SIZE 16

// #define IR_LED_PIN 6;

//================================================
// Global variables
// LCD Screen
int xLocation = 0;
int yLocation = 0;
bool characterMovable = true;
bool turnAround = false;

display d;

// IR

volatile bool isInterrupt = false;
volatile bool fullPulseArray = false;
volatile bool validBit = false;
volatile bool receiveOk = false;

volatile uint32_t pulseArrayCounter = 0;

int pulseArray[ARRAY_SIZE];
int bitArray[ARRAY_SIZE];
int testArray[ARRAY_SIZE] = {1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0}; // Last bit not used

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
void drawHookIdle(int xLocation);

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

    d.generateItems();

    // Eindeloze lus
    while (1)
    {
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

        if (Nunchuk.state.c_button == 1)
        {
            characterMovable = false;
            drawHookIdle(xLocation);
        }
    }

    return 0;
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
            drawHookIdle(xLocation);
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

void drawHookIdle(int xLocation)
{
    int xOrigin = xLocation + 25;
    int yOrigin = 80;

    // Calculate the radius of the circle
    int radius = 15;

    // Calculate the angle step to draw points on the circle
    float angleStep = 0.05; // Change this to adjust the spacing of points on the circle

    for (float angle = 0; angle < 1 * PI; angle += angleStep)
    {
        Nunchuk.getState(NUNCHUK_ADDRESS);
        if (Nunchuk.state.z_button == 1)
        {
            for (int i = 15; i < 400; i++)
            {
                // Calculate the coordinates on the circle using polar coordinates
                int xCircle = xOrigin + (int)(i * cos(angle));
                int yCircle = yOrigin + (int)(i * sin(angle));

                // Draw lines from origin to points on the circle
                d.drawHook(xOrigin, yOrigin, xCircle, yCircle);
                _delay_ms(25);
                if (yCircle == 240 || xCircle == 0 || xCircle == 320)
                {
                    turnAround = true;
                    int xStopMoment = xCircle;
                    int yStopMoment = yCircle;
                    for (int i2 = 0; i2 < i; i2++)
                    {
                        if (xCircle == xOrigin)
                        {
                            break;
                        }
                        int xCircle = xStopMoment - (int)(i2 * cos(angle));
                        int yCircle = yStopMoment - (int)(i2 * sin(angle));

                        d.removeHook(xStopMoment, yStopMoment, xCircle, yCircle);

                        _delay_ms(25);
                    }
                }
                if (turnAround)
                {
                    turnAround = false;
                    break;
                }
            }
            characterMovable = true;
        }
        else
        {
            // Calculate the coordinates on the circle using polar coordinates
            int xCircle = xOrigin + (int)(radius * cos(angle));
            int yCircle = yOrigin + (int)(radius * sin(angle));

            // Draw lines from origin to points on the circle
            d.drawHook(xOrigin, yOrigin, xCircle, yCircle);
            _delay_ms(20);
            d.removeHook(xOrigin, yOrigin, xCircle, yCircle);
        }
    }
    if (!characterMovable)
    {
        for (float angle = PI; angle > 0; angle -= angleStep)
        {
            Nunchuk.getState(NUNCHUK_ADDRESS);
            if (Nunchuk.state.z_button == 1)
            {
                for (int i = 15; i < 400; i++)
                {
                    // Calculate the coordinates on the circle using polar coordinates
                    int xCircle = xOrigin + (int)(i * cos(angle));
                    int yCircle = yOrigin + (int)(i * sin(angle));

                    // Draw lines from origin to points on the circle
                    d.drawHook(xOrigin, yOrigin, xCircle, yCircle);
                    _delay_ms(25);
                    if (yCircle == 240 || xCircle == 0 || xCircle == 320)
                    {
                        turnAround = true;
                        int xStopMoment = xCircle;
                        int yStopMoment = yCircle;
                        for (int i2 = 0; i2 < i; i2++)
                        {
                            if (xCircle == xOrigin)
                            {
                                break;
                            }
                            int xCircle = xStopMoment - (int)(i2 * cos(angle));
                            int yCircle = yStopMoment - (int)(i2 * sin(angle));

                            d.removeHook(xStopMoment, yStopMoment, xCircle, yCircle);
                            _delay_ms(25);
                        }
                    }
                    if (turnAround)
                    {
                        turnAround = false;
                        break;
                    }
                }
            }
            else
            {
                // Calculate the coordinates on the circle using polar coordinates
                int xCircle = xOrigin + (int)(radius * cos(angle));
                int yCircle = yOrigin + (int)(radius * sin(angle));

                // Draw lines from origin to points on the circle
                d.drawHook(xOrigin, yOrigin, xCircle, yCircle);
                _delay_ms(20);
                d.removeHook(xOrigin, yOrigin, xCircle, yCircle);
            }
        }
        characterMovable = true;
    }

    characterMovable = true;
}
