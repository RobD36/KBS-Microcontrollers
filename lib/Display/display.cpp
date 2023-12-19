#include "display.h"
#include "items.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(10, 9, -1);


display::display() {}

void display::init()
{
    // Initialisatie van het LCD-scherm
    tft.begin();
    tft.setRotation(1); // Pas dit aan afhankelijk van de oriëntatie van het scherm

    // Voorbeeld: Tekst "Hello, World!" weergeven op het scherm
    tft.fillScreen(COLOR_BACKGROUND);

}

void display::displayFillScreen()
{
    tft.fillScreen(COLOR_BACKGROUND);
}

void display::displayCharacter(int x, int y)
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

void display::resetSkyRight(int xLocation)
{
    tft.fillRect(xLocation + 50, 23, 10, 57, COLOR_BACKGROUND); // right side
    tft.fillRect(xLocation + 30, 23, 25, 37, COLOR_BACKGROUND); // shoulder and face
    tft.fillRect(xLocation, 75, 50, 5, COLOR_BACKGROUND);       // underneath minecart
}

void display::resetSkyLeft(int xLocation)
{
    tft.fillRect(xLocation - 10, 23, 10, 57, COLOR_BACKGROUND); // left side
    tft.fillRect(xLocation, 23, 25, 37, COLOR_BACKGROUND);      // shoulder and face
    tft.fillRect(xLocation, 75, 50, 5, COLOR_BACKGROUND);       // underneath minecart
}

void display::generateItems(Item items[])
{
    for(int i = 0; i < 9; i++) {
        Item item = items[i];
        switch (item.type)
        {
        case GOLD:
             tft.fillRect(item.x, item.y, item.size, item.size, COLOR_GOLD);
            break;
        case STONE:
             tft.fillRect(item.x, item.y, item.size, item.size, COLOR_ROCK);
            break;
        case DIAMOND:
             tft.fillRect(item.x, item.y, item.size, item.size, COLOR_DIAMOND);
        }
    }
};

void display::removeHook(int xBegin, int yBegin, int xEnd, int yEnd)
{
    tft.drawLine(xBegin, yBegin, xEnd, yEnd, COLOR_BROWN);
    tft.drawLine(xBegin + 1, yBegin + 1, xEnd + 1, yEnd + 1, COLOR_BROWN);
    tft.drawLine(xBegin - 1, yBegin - 1, xEnd - 1, yEnd - 1, COLOR_BROWN);
}

void display::removeHookSquare(int xBegin, int yBegin, int size)
 {
    tft.fillRect(xBegin - (size/2), yBegin - (size/2), size, size, COLOR_BROWN);
 }

void display::drawHook(int xBegin, int yBegin, int xEnd, int yEnd)
{
    tft.drawLine(xBegin, yBegin, xEnd, yEnd, ILI9341_BLACK);
}

void display::drawItemWhenGrabbed(int xBegin, int yBegin, int size, ItemType type) {
    if(type == GOLD) {
        tft.fillRect(xBegin - (size/2), yBegin - (size/2), size, size, COLOR_GOLD);
    }
    else if(type == STONE) {
        tft.fillRect(xBegin - (size/2), yBegin - (size/2), size, size, COLOR_ROCK);
    }
    else if(type == DIAMOND) {
        tft.fillRect(xBegin - (size/2), yBegin - (size/2), size, size, COLOR_DIAMOND);
    }
}

void display::removeItem(int xBegin, int yBegin, int size) {
    tft.fillRect(xBegin, yBegin, size, size, COLOR_BROWN);
}

void display::updateScore(int valueItem) {
    score += valueItem; // Update the score
    tft.fillRect(250, 5, 100, 10, COLOR_BACKGROUND); // Clear previous score
    tft.setCursor(250, 5);
    tft.print("You: $");
    tft.print(String(score));
}

void display::displayItemValue(int valueItem) {
    tft.setCursor(135, 10);
    tft.setTextColor(COLOR_MONEY);
    tft.setFont(&FreeSerifBoldItalic9pt7b);
    tft.print("$");
    tft.print(String(valueItem));
    tft.setTextColor(ILI9341_BLACK);
    tft.setFont(NULL);
    _delay_ms(250);
    // tft.fillRect(134, 0, 40, 20, COLOR_BACKGROUND);
    for(int i = 0; i < 20; i++) {
        tft.fillRect(134, 0, 40, i, COLOR_BACKGROUND);
        _delay_ms(5);
    }
}

void display::displayStartMenu() {
    displayFillScreen();
    //Display Start/Highscores
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setFont(NULL);
    tft.setCursor(60, 140);
    tft.print("Start");
    tft.setCursor(60, 160);
    tft.print("Highscores");

    //Display character
    displayCharacter(230, 160);
    //Display logo
    menuLogo();
}

void display::menuLogo()
{
    //Display logo
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

    //Display text
    tft.setTextSize(2);
    tft.setCursor(65,67);
    tft.setFont(&FreeSerifBoldItalic9pt7b);
    tft.print("Goudzoekers");
}

void display::startMenuCursor(bool cursorPosition) {
    if(cursorPosition)
    {
        //Highscores
        tft.drawRect(50, 137, 77, 20, COLOR_BACKGROUND);
        tft.drawRect(50, 157, 140, 20, ILI9341_BLACK);
    }
    else
    {
        //Start
        tft.drawRect(50, 157, 140, 20, COLOR_BACKGROUND);
        tft.drawRect(50, 137, 77, 20, ILI9341_BLACK);
    }
}

void display::displayLevel()
{
    tft.setFont(NULL);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(1);
    tft.setCursor(5, 5);
    tft.print("Time: 120");
    tft.setCursor(250, 5);
    tft.print("You: $");
    tft.print(score);
    tft.setCursor(220, 15);
    tft.print("Opponent: $400");
    tft.fillRect(0, 80, 320, 300, COLOR_BROWN);
}

void display::displayHighscore()
{
    tft.fillScreen(COLOR_LOGO_BROWN);
    int x = 60;
    int y = 60;

    highscore hs;
    hs.sortHighscore();
    int* array = hs.loadHighscore();

    tft.setCursor(60, 40);
    tft.setTextSize(2);
    tft.setFont(NULL);
    tft.print("Highscores:");

    for(int i = 0; i < 5; i++)
    {

        tft.setCursor(x, y);
        tft.setTextSize(2);
        tft.print(i + 1);
        tft.setCursor(x + 8, y);
        tft.print(".");
        tft.setCursor(x + 25, y);
        tft.print(array[i]);
        y += 20;
    }

    tft.setTextSize(2);
    tft.setCursor(10, 200);
    tft.print("back");

    tft.setCursor(10, 220);
    tft.print("reset highscores");
}

void display::displayHighscoreDecorative()
{
    displayCharacter(200, 100);

    // items left
    tft.fillRect(0, 0, 40, 40, COLOR_GOLD);                     //5
    tft.drawRect(0, 0, 40, 40, ILI9341_ORANGE);
    tft.drawRect(1, 1, 38, 38, ILI9341_ORANGE);

    tft.fillTriangle(20, 0, 45, 50, 70, 0, COLOR_ROCK);         //6
    tft.drawTriangle(20, 0, 45, 50, 70, 0, COLOR_WHEELS);
    tft.drawTriangle(21, 1, 45, 49, 69, 1, COLOR_WHEELS);

    tft.fillRect(0, 30, 30, 30, COLOR_GOLD);                    //3
    tft.drawRect(0, 30, 30, 30, ILI9341_ORANGE);
    tft.drawRect(1, 31, 28, 28, ILI9341_ORANGE);

    tft.fillRect(0, 25, 10, 10, COLOR_DIAMOND);                 //4
    tft.drawRect(0, 25, 10, 10, COLOR_DIAMOND_RAND);

    tft.fillTriangle(0, 50, 30, 70, 0, 90, COLOR_ROCK);         //2
    tft.drawTriangle(0, 50, 30, 70, 0, 90, COLOR_WHEELS);
    tft.drawTriangle(1, 51, 29, 70, 1, 89, COLOR_WHEELS);

    tft.fillRect(0, 85, 10, 10, COLOR_DIAMOND);                 //1
    tft.drawRect(0, 85, 10, 10, COLOR_DIAMOND_RAND);

    //items right above
    tft.fillTriangle(280, 0, 320, 0, 320, 40, COLOR_GOLD);      //10
    tft.drawTriangle(280, 0, 320, 0, 320, 40, ILI9341_ORANGE);
    tft.drawTriangle(281, 1, 319, 1, 319, 39, ILI9341_ORANGE);

    tft.fillRect(250, -2, 40, 20, COLOR_ROCK);                   //9
    tft.drawRect(250, -2, 40, 20, COLOR_WHEELS);
    tft.drawRect(251, -1, 38, 18, COLOR_WHEELS);

    tft.fillTriangle(220, 0, 240, 30, 260, 0, COLOR_GOLD);      //8
    tft.drawTriangle(220, 0, 240, 30, 260, 0, ILI9341_ORANGE);
    tft.drawTriangle(219, 1, 240, 29, 259, 1, ILI9341_ORANGE);

    tft.fillRect(215, 0, 10, 10, COLOR_DIAMOND);                //7
    tft.drawRect(215, 0, 10, 10, COLOR_DIAMOND_RAND);

    tft.fillRect(290, 60, 30, 30, COLOR_GOLD);                  //13
    tft.drawRect(290, 60, 30, 30, ILI9341_ORANGE);
    tft.drawRect(291, 61, 29, 29, ILI9341_ORANGE);

    tft.fillTriangle(320, 30, 290, 50, 320, 70, COLOR_ROCK);    //12
    tft.drawTriangle(320, 30, 290, 50, 320, 70, COLOR_WHEELS);
    tft.drawTriangle(319, 31, 291, 50, 319, 69, COLOR_WHEELS);

    tft.fillRect(310, 30, 10, 10, COLOR_DIAMOND);               //11
    tft.drawRect(310, 30, 10, 10, COLOR_DIAMOND_RAND);

    tft.fillTriangle(320, 90, 280, 110, 320, 130, COLOR_ROCK);  //15
    tft.drawTriangle(320, 90, 280, 110, 320, 130, COLOR_WHEELS);
    tft.drawTriangle(319, 91, 281, 110, 319, 129, COLOR_WHEELS);

    tft.fillRect(300, 80, 20, 20, COLOR_GOLD);                  //14
    tft.drawRect(300, 80, 20, 20, ILI9341_ORANGE);
    tft.drawRect(301, 81, 19, 19, ILI9341_ORANGE);

    //items right below
    tft.fillRect(290, 160, 30, 30, COLOR_GOLD);                 //17
    tft.drawRect(290, 160, 30, 30, ILI9341_ORANGE);
    tft.drawRect(291, 161, 28, 28, ILI9341_ORANGE);

    tft.fillRect(310, 155, 10, 10, COLOR_DIAMOND);              //16
    tft.fillRect(310, 155, 10, 10, COLOR_DIAMOND_RAND);

    tft.fillRect(300, 180, 30, 30, COLOR_ROCK);                 //18
    tft.drawRect(300, 180, 30, 30, COLOR_WHEELS);
    tft.drawRect(301, 181, 28, 28, COLOR_WHEELS);

    tft.fillRect(280, 210, 60, 40, COLOR_GOLD);                  //19
    tft.drawRect(280, 210, 60, 40, ILI9341_ORANGE);
    tft.drawRect(281, 211, 58, 38, ILI9341_ORANGE);

    tft.fillTriangle(250, 240, 270, 210, 290, 240, COLOR_GOLD); //21
    tft.drawTriangle(251, 239, 270, 211, 289, 239, ILI9341_ORANGE);

    tft.fillRect(285, 230, 10, 10, COLOR_DIAMOND);              //20
    tft.drawRect(285, 230, 10, 10, COLOR_DIAMOND_RAND);
}

void display::highscoreCursor(bool cursorPosition) {
    if(cursorPosition)
    {
        //Reset highscores
        tft.drawRect(5, 197, 60, 20, COLOR_LOGO_BROWN);
        tft.drawRect(5, 217, 200, 20, ILI9341_BLACK);
    }
    else
    {
        //Back
        tft.drawRect(5, 217, 200, 20, COLOR_LOGO_BROWN);
        tft.drawRect(5, 197, 60, 20, ILI9341_BLACK);
    }
}