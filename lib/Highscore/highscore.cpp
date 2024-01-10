#include "highscore.h"

highscore::highscore() {}

void highscore::saveHighscore(int score)
{
    int* array = loadHighscore();
    
    for (int i = 4; i > 0; i--)
    {
        if (score > array[i])
        {
            array[i] = score;
            EEPROM.write(i * 2, score);

            EEPROM.write(i * 2, score >> 8);
            EEPROM.write((i * 2) + 1, score & 0xFF);
            break;
        }
    }
}

int* highscore::loadHighscore()
{
    static int highscoreArray[5]; // = {64, 5, 313, 23, 1};
    for (int i = 0; i < 4; i++)
    {
        highscoreArray[i] = EEPROM.read(i * 2) << 8 | EEPROM.read(i * 2 + 1);
    }
    return highscoreArray;
}

void highscore::sortHighscore()
{
    int* array = loadHighscore();
    int i, j, k;

    for(i = 0; i < 4; i++)
    {
        for(j = i + 1; j < 4; j++)
        {
            if(array[j] > array[i])
            {
                int temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }

    for(k = 0; k < 5; k++)
    {
        EEPROM.write(k * 2, array[k] >> 8);
        EEPROM.write((k * 2) + 1, array[k] & 0xFF);
    }
}

void highscore::resetHighscores()
{
    for (int i = 0; i < 5; i++)
    {
        EEPROM.write(i * 2, 0);
        EEPROM.write((i * 2) + 1, 0);
    }
}