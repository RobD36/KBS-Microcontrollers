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
bool characterMovable = true;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void draw_pixel()
{
    tft.fillCircle(xLocation, yLocation, 5, ILI9341_CASET);
}

void erase_pixel()
{
    tft.fillCircle(xLocation, yLocation, 5, ILI9341_MAGENTA);
}

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
            // Calculate the coordinates on the circle using polar coordinates
            int xCircle = xOrigin + (int)(400 * cos(angle));
            int yCircle = yOrigin + (int)(400 * sin(angle));

            // Draw lines from origin to points on the circle
            tft.drawLine(xOrigin, yOrigin, xCircle, yCircle, ILI9341_BLACK);
            break;
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

    for (float angle = PI; angle > 0; angle -= angleStep)
    {
        Nunchuk.getState(NUNCHUK_ADDRESS);
        if (Nunchuk.state.z_button == 1)
        {
            // Calculate the coordinates on the circle using polar coordinates
            int xCircle = xOrigin + (int)(400 * cos(angle));
            int yCircle = yOrigin + (int)(400 * sin(angle));

            // Draw lines from origin to points on the circle
            tft.drawLine(xOrigin, yOrigin, xCircle, yCircle, ILI9341_BLACK);
            break;
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

int main(void)
{
    // Initialisatie van het LCD-scherm
    tft.begin();
    tft.setRotation(1); // Pas dit aan afhankelijk van de oriëntatie van het scherm

    // Voorbeeld: Tekst "Hello, World!" weergeven op het scherm
    tft.fillScreen(COLOR_BACKGROUND);
    sei();

    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(1);
    tft.setCursor(5, 5);
    tft.print("Time: 120");
    tft.setCursor(250, 5);
    tft.print("You: $500");
    tft.setCursor(220, 15);
    tft.print("Opponent: $400");
    tft.fillRect(0, 80, 320, 300, COLOR_BROWN);
    tft.fillRect(100, 100, 10, 10, COLOR_GOLD);
    tft.fillRect(200, 200, 10, 10, COLOR_GOLD);
    tft.fillRect(180, 150, 10, 10, COLOR_ROCK);

    displayCharacter(xLocation, 55);

    // use Serial for printing nunchuk data
    Serial.begin(BAUDRATE);

    // join I2C bus as master
    Wire.begin();

    // Eindeloze lus
    while (1)
    {
        if (!Nunchuk.getState(NUNCHUK_ADDRESS))
            return (false);

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
    }

    return 0;
}
