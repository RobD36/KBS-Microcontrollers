#include "IRLib.h"

// =================================================
// Global variables

int IRledPin = 6;

// =================================================
// Pre defines of functions

void printIntArray(int array[], int size);
void IRpulse(long microSeconds);
void delayTimer(uint16_t microSeconds);
void sendSignal(int array[], int size);
void sendTestSignal();

// =================================================
// Functions

void printIntArray(int array[], int size)
{
    for(uint16_t i = 0; i < size; i++)
    {
        Serial.print(array[i]);
        Serial.print(" ");
    }
    Serial.println();
}

void IRpulse(long microSeconds)
{
    while(microSeconds > 0)
    {
        PORTD |= (1 << IRledPin); // 3 Microseconds
        delayTimer(10);
        PORTD &= ~(1 << IRledPin); // 3 Microseconds
        delayTimer(10);
        microSeconds -= 26;
    }
}

void delayTimer(uint16_t microSeconds)
{
    TCNT2 = 0; // Reset Timer2
    TCCR2B |= (1 << CS20); // Set prescaler to 1

    for (uint16_t i = 0; i < microSeconds; i++) // Wait for amount microseconds
    {
        while (!(TIFR2 & (1 << OCF2A))); // Wait for Timer2 compare flag

        TIFR2 |= (1 << OCF2A); // Clear Timer2 compare flag
    }

    TCCR2B &= ~(1 << CS20); // Stop Timer2
}

void sendSignal(int array[], int size)
{
    cli(); // Disable global interrupts
    //Start pulse
    IRpulse(9000);
    delayTimer(4500);

    //1 = 1687 uS
    //0 = 562 uS
    //Data 
    for(uint16_t i = 0; i < size; i++)
    {
        if(array[i] == 1)
        {
            IRpulse(562); delayTimer(1687); // 1
        }
        else
        {
            IRpulse(562); delayTimer(562);  // 0
        }
    }
    IRpulse(562); // End pulse
    sei(); // Enable global interrupts
}

//Hard coded test IR signal
void sendTestSignal()
{
    cli(); // Disable global interrupts
    //Start pulse
    IRpulse(9000);
    delayTimer(4500);

    //1 = 1687 uS
    //0 = 562 uS
    //Data 
    IRpulse(562); delayTimer(1687); // 1
    IRpulse(562); delayTimer(562);  // 0
    IRpulse(562); delayTimer(562);  // 0
    IRpulse(562); delayTimer(1687); // 1
    IRpulse(562); delayTimer(1687); // 1
    IRpulse(562); delayTimer(1687); // 1
    IRpulse(562); delayTimer(562);  // 0
    IRpulse(562); delayTimer(562);  // 0
    IRpulse(562); delayTimer(562);  // 0
    IRpulse(562); delayTimer(562);  // 0
    IRpulse(562); delayTimer(1687); // 1
    IRpulse(562); delayTimer(1687); // 1
    IRpulse(562); delayTimer(562);  // 0
    IRpulse(562); delayTimer(562);  // 0
    IRpulse(562); delayTimer(1687); // 1
    IRpulse(562); delayTimer(1687); // 1
    IRpulse(562);   

    sei(); // Enable global interrupts
}