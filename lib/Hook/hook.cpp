#include "hook.h"
#include "display.h"

display c;

hook::hook() {}

void hook::calculateAndDrawHook(int xOrigin, int yOrigin, float angle, Item items[])
{
    for (int i = 15; i < 400; i++)
    {
        // Calculate the coordinates on the circle using polar coordinates
        int xCircle = xOrigin + (int)(i * cos(angle));
        int yCircle = yOrigin + (int)(i * sin(angle));

        // Draw lines from origin to points on the circle
        c.drawHook(xOrigin, yOrigin, xCircle, yCircle);
        _delay_ms(5);

        for (int j = 0; j < 9; j++)
        {
            Item &item = items[j];
            if ((xCircle > item.x && xCircle < (item.x + item.size)) && (yCircle > item.y && yCircle < (item.y + item.size)))
            {
                turnAround = true;
                int xStopMoment = xCircle;
                int yStopMoment = yCircle;

                c.removeItem(item.x, item.y, item.size);
                    item.x = 0;
                    item.y = 0;

                for (int i2 = 0; i2 < i - (15 + item.size/2); i2++)
                {
                    int xCircle = xStopMoment - (int)(i2 * cos(angle));
                    int yCircle = yStopMoment - (int)(i2 * sin(angle));

                    c.drawItemWhenGrabbed(xCircle, yCircle, item.size, item.type);

                    if(item.size < 10) {
                        _delay_ms(3);
                    } else if (item.size < 20) {
                        _delay_ms(10);
                    } else if (item.size < 30) {
                        _delay_ms(20);
                    } else if (item.size < 40) {
                        _delay_ms(40);
                    } else {
                        _delay_ms(80);
                    }

                    c.removeHookSquare(xCircle, yCircle, item.size);

                }
                c.displayItemValue(item.value);
                c.updateScore(item.value);
            }
        }

        if (yCircle == 240 || xCircle == 0 || xCircle == 320)
        {
            turnAround = true;
            int xStopMoment = xCircle;
            int yStopMoment = yCircle;
            for (int i2 = 0; i2 < i - 15; i2++)
            {
                int xCircle = xStopMoment - (int)(i2 * cos(angle));
                int yCircle = yStopMoment - (int)(i2 * sin(angle));

                c.removeHook(xStopMoment, yStopMoment, xCircle, yCircle);

                _delay_ms(5);
            }
        }
        if (turnAround)
        {
            turnAround = false;
            break;
        }
    }
}

void hook::removeHook(int xOrigin, int yOrigin, int radius, float angle)
{
    // Calculate the coordinates on the circle using polar coordinates
    int xCircle = xOrigin + (int)(radius * cos(angle));
    int yCircle = yOrigin + (int)(radius * sin(angle));

    // Draw lines from origin to points on the circle
    c.drawHook(xOrigin, yOrigin, xCircle, yCircle);
    _delay_ms(20);
    c.removeHook(xOrigin, yOrigin, xCircle, yCircle);
}