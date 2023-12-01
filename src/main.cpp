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
    tft.setRotation(1); // Pas dit aan afhankelijk van de oriëntatie van het scherm

    // Voorbeeld: Tekst "Hello, World!" weergeven op het scherm
    tft.fillScreen(BACKGROUND);
    sei();

    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(1);
    tft.setCursor(5, 5);
    tft.print("Time: 120");
    tft.setCursor(250, 5);
    tft.print("You: $500");
    tft.setCursor(220, 15);
    tft.print("Opponent: $400");
    tft.fillRect(0, 80, 320, 300, BROWN);
    tft.fillRect(100, 100, 10, 10, GOLD);
    tft.fillRect(200, 200, 10, 10, GOLD);
    tft.fillRect(180, 150, 10, 10, ROCK);


    tft.setTextColor(ILI9341_BLUE);
    tft.setTextSize(2);
    tft.setCursor(120, 60);
    tft.cp437(true);   // Use correct CP437 character codes
    tft.write(0x02);   // Print the o-with-umlauts

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
