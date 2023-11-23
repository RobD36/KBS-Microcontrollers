#include <avr/io.h>
#include <util/delay.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <Nunchuk.h>

#define NUNCHUK_ADDRESS 0x52
#define WAIT 1000
#define BAUDRATE 9600
#define CHUNKSIZE 32
#define BUFFERLEN 256

#define TFT_CS 10
#define TFT_RST -1
#define TFT_DC 9

int xLocation = 0;
int yLocation = 0;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void draw_pixel()
{
    tft.fillCircle(xLocation, yLocation, 5, ILI9341_CASET);
}

void erase_pixel()
{
    tft.fillCircle(xLocation, yLocation, 5, ILI9341_MAGENTA);
}

int main(void)
{
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
    }

    return 0;
}
