#include <util/delay.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <Nunchuk.h>
#include <IRLib.h>

#define NUNCHUK_ADDRESS 0x52
#define WAIT 1000
#define BAUDRATE 9600
#define CHUNKSIZE 32
#define BUFFERLEN 256

#define TFT_CS 10
#define TFT_RST -1
#define TFT_DC 9

#define ARRAY_SIZE 16

//#define IR_LED_PIN 6;

//================================================
//Global variables
//LCD Screen
int xLocation = 0;
int yLocation = 0;
bool characterMovable = true;
bool turnAround = false;
int coordsBlocks[3][3];

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

//IR

volatile bool isInterrupt = false;
volatile bool fullPulseArray = false;
volatile bool validBit = false;
volatile bool receiveOk = false;

volatile uint32_t pulseArrayCounter = 0;

int pulseArray[ARRAY_SIZE];
int bitArray[ARRAY_SIZE];
int testArray[ARRAY_SIZE] = {1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0}; //Last bit not used


//================================================
//Pre defines of functions
//Nunchuck
bool readNunchuck();

//LCD screen
void drawCircle();
void eraseCircle();

//IR
void convertArray();


//================================================
//Interrupts
ISR(INT0_vect)
{
    if(PIND & (1 << PD2))
    {
        //Debugging
        //receiveOk = true;
        if(TCNT1 > 3000) //Receive start signal
        { 
            pulseArrayCounter = 0;
        } 
        else 
        {
            pulseArray[pulseArrayCounter] = TCNT1; //Signal into pulseArray
            pulseArrayCounter++;
        }
        TCNT1 = 0;
        if (pulseArrayCounter == ARRAY_SIZE)
        {
            fullPulseArray = true;
            isInterrupt = true;
        }    
    }
    else{
        //Falling edge (Not used)
    }
}

<<<<<<< HEAD
//================================================
//Main
=======
void displayCharacter(int x, int y)
{
    tft.fillCircle(x + 25, y - 20, 10, COLOR_SKIN); // face
    tft.drawPixel(x + 22, y - 23, ILI9341_BLACK);   // eyes
    tft.drawPixel(x + 28, y - 23, ILI9341_BLACK);
    tft.drawFastVLine(x + 26, y - 32, 3, ILI9341_BLACK); // hair
    tft.drawFastVLine(x + 22, y - 32, 3, ILI9341_BLACK);
    tft.drawFastVLine(x + 28, y - 32, 3, ILI9341_BLACK);
    tft.fillRect(x + 5, y - 10, 40, 10, COLOR_BODY); // shirt
    tft.fillRect(x + 10, y - 13, 30, 3, COLOR_BODY);
    tft.fillRect(x, y, 50, 20, COLOR_MINECART); // minecart
    tft.fillRect(x + 5, y + 5, 40, 10, COLOR_BROWN);
    tft.fillRect(x + 5, y + 9, 40, 2, COLOR_MINECART);
    tft.fillCircle(x + 10, y + 20, 4, COLOR_WHEELS); // wheels
    tft.fillCircle(x + 40, y + 20, 4, COLOR_WHEELS);
    tft.fillCircle(x + 10, y, 5, COLOR_SKIN); // hands
    tft.fillCircle(x + 40, y, 5, COLOR_SKIN);
}

void resetSkyRight()
{
    tft.fillRect(xLocation + 50, 23, 10, 57, COLOR_BACKGROUND); // right side
    tft.fillRect(xLocation + 30, 23, 25, 37, COLOR_BACKGROUND); // shoulder and face
    tft.fillRect(xLocation, 75, 50, 5, COLOR_BACKGROUND);       // underneath minecart
}

void resetSkyLeft()
{
    tft.fillRect(xLocation - 10, 23, 10, 57, COLOR_BACKGROUND); // left side
    tft.fillRect(xLocation, 23, 25, 37, COLOR_BACKGROUND);      // shoulder and face
    tft.fillRect(xLocation, 75, 50, 5, COLOR_BACKGROUND);       // underneath minecart
}

void createBlocks(int Small, int Medium, int big)
{
    tft.fillRect(100, 100, 10, 10, COLOR_GOLD);
    tft.fillRect(200, 200, 10, 10, COLOR_GOLD);
    tft.fillRect(180, 150, 10, 10, COLOR_ROCK);

    coordsBlocks[0][0] = 100;
    coordsBlocks[0][1] = 100;
    coordsBlocks[0][2] = 10;

    coordsBlocks[1][0] = 200;
    coordsBlocks[1][1] = 200;
    coordsBlocks[1][2] = 10;

    coordsBlocks[2][0] = 180;
    coordsBlocks[2][1] = 150;
    coordsBlocks[2][2] = 10;
}

void drawHookIdle()
{
    int xOrigin = xLocation + 25;
    int yOrigin = 80;
    // tft.drawLine(xOrigin, yOrigin, xDestination, yDestination, ILI9341_BLACK);

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
                tft.drawLine(xOrigin, yOrigin, xCircle, yCircle, ILI9341_BLACK);
                _delay_ms(25);
                if (yCircle == 240 || xCircle == 0 || xCircle == 320 || ((xCircle > coordsBlocks[0][0] && xCircle < (coordsBlocks[0][0] + 10)) && (yCircle > coordsBlocks[0][1] && yCircle < (coordsBlocks[0][1] + 10))))
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

                        tft.drawLine(xStopMoment, yStopMoment, xCircle, yCircle, COLOR_BROWN);
                        tft.drawLine(xStopMoment + 1, yStopMoment + 1, xCircle + 1, yCircle + 1, COLOR_BROWN);
                        tft.drawLine(xStopMoment - 1, yStopMoment - 1, xCircle - 1, yCircle - 1, COLOR_BROWN);

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
            tft.drawLine(xOrigin, yOrigin, xCircle, yCircle, ILI9341_BLACK);
            _delay_ms(20);
            tft.fillRect(xLocation, 80, 50, 25, COLOR_BROWN);
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
                    tft.drawLine(xOrigin, yOrigin, xCircle, yCircle, ILI9341_BLACK);
                    _delay_ms(25);
                    if (yCircle == 240 || xCircle == 0 || xCircle == 320 || ((xCircle > coordsBlocks[0][0] && xCircle < (coordsBlocks[0][0] + 10)) && (yCircle > coordsBlocks[0][1] && yCircle < (coordsBlocks[0][1] + 10))))
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

                            tft.drawLine(xStopMoment, yStopMoment, xCircle, yCircle, COLOR_BROWN);
                            tft.drawLine(xStopMoment + 1, yStopMoment + 1, xCircle + 1, yCircle + 1, COLOR_BROWN);
                            tft.drawLine(xStopMoment - 1, yStopMoment - 1, xCircle - 1, yCircle - 1, COLOR_BROWN);

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
                tft.drawLine(xOrigin, yOrigin, xCircle, yCircle, ILI9341_BLACK);
                _delay_ms(20);
                tft.fillRect(xLocation, 80, 50, 25, COLOR_BROWN);
            }
        }
        characterMovable = true;
    }

    characterMovable = true;
}

>>>>>>> main
int main(void)
{
    // Initialise IR sensor pin
    DDRD |= (1 << DDD6);
    // Enable external interrupt 0
    EICRA |= (1 << ISC00);
    EIMSK |= (1 << INT0);
    // Initialise timers
    // Timer 1
    TCCR1B = (1 << CS10) | (1 << CS12); //Set prescaler to 1024
    TCNT1 = 0;
    // Timer 2
    TCCR2A |= (1 << WGM21); // CTC mode
    OCR2A = (F_CPU / 1000000UL) - 1; // Set compare value for 1 microsecond delay
    TIMSK2 |= (1 << OCIE2A); // Enable compare interrupt

    // Initialisatie van het LCD-scherm
    tft.begin();
    tft.setRotation(1); // Pas dit aan afhankelijk van de oriëntatie van het scherm

    // Voorbeeld: Tekst "Hello, World!" weergeven op het scherm
<<<<<<< HEAD
    tft.fillScreen(ILI9341_MAGENTA);
=======
    tft.fillScreen(COLOR_BACKGROUND);
    sei();
>>>>>>> main

    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(1);
    tft.setCursor(5, 5);
    tft.print("Time: 120");
    tft.setCursor(250, 5);
    tft.print("You: $500");
    tft.setCursor(220, 15);
    tft.print("Opponent: $400");
    tft.fillRect(0, 80, 320, 300, COLOR_BROWN);
    createBlocks(0, 3, 0);

    displayCharacter(xLocation, 55);

    // use Serial for printing nunchuk data
    Serial.begin(BAUDRATE);

    // join I2C bus as master
    Wire.begin();

<<<<<<< HEAD
    // Enable global interrupts
    sei();

    // Eindeloze lus
    while (1)
    {   
        /*
        eraseCircle();
        readNunchuck();
        drawCircle();
        */
       
       //Receive
       //if(fullPulseArray && validBit){
       // convertArray();
       //}
       
       //Send
       sendSignal(testArray, sizeof(testArray));
       _delay_ms(2000);
    }

    return 0;
}
//================================================
//Functions
//Nunchuck
bool readNunchuck() {
    if (!Nunchuk.getState(NUNCHUK_ADDRESS))
		return (false);
=======
    // Eindeloze lus
    while (1)
    {

        if (!Nunchuk.getState(NUNCHUK_ADDRESS))
            return (false);
>>>>>>> main

        int intValueX = static_cast<int>(Nunchuk.state.joy_x_axis);
        int intValueY = static_cast<int>(Nunchuk.state.joy_y_axis);

        // move character left and right
        if ((intValueX < 128 && xLocation > 0) && characterMovable)
        {
            xLocation -= 5;
            resetSkyRight();
            displayCharacter(xLocation, 55);
        }
        if ((intValueX > 128 && xLocation < 270) && characterMovable)
        {
            xLocation += 5;
            resetSkyLeft();
            displayCharacter(xLocation, 55);
        }

        if (Nunchuk.state.c_button == 1)
        {
            characterMovable = false;
            drawHookIdle();
        }
<<<<<<< HEAD
        // _delay_ms(50);
}
//LCD
void drawCircle()
{
    tft.fillCircle(xLocation, yLocation, 5, ILI9341_CASET);
}
=======
    }
>>>>>>> main

void eraseCircle()
{
    tft.fillCircle(xLocation, yLocation, 5, ILI9341_MAGENTA);
}

//IR
//Convert pulse array to bit array based on pulse lengths

void convertArray()
{
    if (fullPulseArray)
    {
        //For debugging
        printIntArray(pulseArray, (sizeof(pulseArray)/2));

        for (uint16_t i = 0; i < (sizeof(pulseArray)/2); i ++)
        {
            //Checking for pulse lenghts and set to binary
            if (pulseArray[i] >= 16 && pulseArray[i] <= 20)//Pulse length between 16 & 20 = 0
            {
                bitArray[i] = 0;
            }
            else if(pulseArray[i] >= 33 && pulseArray[i] <= 37)//Pulse length between 33 & 37 = 1
            {
                bitArray[i] = 1;
            }
            pulseArray[i] = 0; //Pulse array reset
        }
        if ((sizeof(bitArray)/2) == 16)
        {
            validBit = true;
        }
        pulseArrayCounter = 0; //Reset pulse counter
        
        //For debugging
        printIntArray(bitArray, (sizeof(bitArray)/2));
    }
}

