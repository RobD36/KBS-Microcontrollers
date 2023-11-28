#include <util/delay.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <Nunchuk.h>

#define NUNCHUK_ADDRESS 0x52
#define WAIT 1000
#define BAUDRATE 9600
#define CHUNKSIZE 32
#define BUFFERLEN 256

#define TFT_CS 10
#define TFT_RST -1
#define TFT_DC 9

//#define IR_LED_PIN 6;

//================================================
//Global variables
//LCD Screen
int xLocation = 0;
int yLocation = 0;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

//IR
volatile bool isInterrupt = false;
volatile bool fullPulseArray = false;
volatile bool validBit = false;
volatile bool receiveOk = false;

volatile uint32_t pulseArrayCounter = 0;

int pulseArray[17];
int bitArray[16];
int IR_LED_PIN = 6;

//================================================
//Pre defines of functions
//LCD screen
void draw_pixel();
void erase_pixel();

//IR
void convertArray();
void printBit();
void printArray();
void IRpulse();
void delayTimer();
void sendTestSignal();

//================================================
//Interrupts
ISR(INT0_vect)
{
    if(PIND & (1 << PD2))
    {
        //Debugging
        //receiveOk = true;
        if(TCNT1 > 2000) //Receive start signal
        { 
            pulseArrayCounter = 0;
        } 
        else 
        {
            pulseArray[pulseArrayCounter] = TCNT1; //Signal into pulseArray
            pulseArrayCounter++;
        }
        TCNT1 = 0;
        if (pulseArrayCounter == 16)
        {
            fullPulseArray = true;
            isInterrupt = true;
        }    
    }
    else{
        //Falling edge (Not used)
    }
}

//================================================
//Main
int main(void)
{
    // Initialise IR sensor pin
    DDRD &= ~(1 << DDD2);
    PORTD |= (1 << PORTD2);
    // Enable external interrupt 0
    EICRA |= (1 << ISC00);
    EIMSK |= (1 << INT0);
    // Initialise timers
    // Timer 1
    TCCR1B = (1 << CS10) | (1 << CS12); //Set prescaler to 1024
    TCNT1 = 0;
    // Timer 2
    TCCR2B = (1 << CS10) | (1 << CS12); 
    TCNT2 = 0;


    // Initialisatie van het LCD-scherm
    tft.begin();
    tft.setRotation(3); // Pas dit aan afhankelijk van de oriëntatie van het scherm

    // Voorbeeld: Tekst "Hello, World!" weergeven op het scherm
    tft.fillScreen(ILI9341_MAGENTA);
    sei();

    // use Serial for printing nunchuk data
    Serial.begin(BAUDRATE);

    // join I2C bus as master
    Wire.begin();



    // Eindeloze lus
    while (1)
    {
        /*
         if (!Nunchuk.getState(NUNCHUK_ADDRESS))
		return (false);

        erase_pixel();

        int intValueX = static_cast<int>(Nunchuk.state.joy_x_axis);
        int intValueY = static_cast<int>(Nunchuk.state.joy_y_axis);

        if(intValueX < 128 && xLocation > 0) {
            xLocation--;
        }
        if(intValueX > 128 && xLocation < 320) {
            xLocation++;
        }
        if(intValueY > 128 && yLocation > 0) {
            yLocation--;
        }
        if(intValueY < 128 && yLocation < 240) {
            yLocation++;
        }
       
        // _delay_ms(50);
        draw_pixel();
        */
        convertArray();
    }

    return 0;
}
//================================================
//Functions
//LCD
void draw_pixel()
{
    tft.fillCircle(xLocation, yLocation, 5, ILI9341_CASET);
}

void erase_pixel()
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
        printArray();

        for (uint16_t i = 0; i < (sizeof(pulseArray)/2); i ++)
        {
            //Checking for pulse lenghts and set to binary
            if (pulseArray[i] >= 16 && pulseArray[i] <= 20)//pulse length between 16 & 20 = 0
            {
                bitArray[i] = 0;
            }
            else if(pulseArray[i] >= 33 && pulseArray[i] <= 37)//pulse length between 33 & 37 = 1
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
        printBit();
    }
}

//Print bit array for debugging
void printBit()
{
    if (validBit)
    {
        for(uint16_t i = 0; i < 17; i++)
        {
            Serial.print(bitArray[i]);
            Serial.print(" ");
        }
        Serial.println();

        validBit = false;
        fullPulseArray = false;
    }
}

//Print pulse array for debugging
void printArray()
{
    for(uint16_t i = 0; i < 17; i++)
    {
        Serial.print(pulseArray[i]);
        Serial.print(" ");
    }
    Serial.println();
}
