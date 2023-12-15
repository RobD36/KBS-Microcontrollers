#include "gamelogic.h"

gamelogic::gamelogic() {}

void gamelogic::getSeconds(long time)
{
    seconds = time;
}

void gamelogic::getMilliseconds(long time)
{
    milliSeconds = time;
}

int *gamelogic::gameTick(Item itemsArray[])
{
    Nunchuk.getState(NUNCHUK_ADDRESS);
    // debounce c button
    if (Nunchuk.state.c_button == 0 && justChanged)
    {
        justChanged = false;
    }

    moveCharacter();

    // switch character mode
    if (Nunchuk.state.c_button == 1 && !justChanged)
    {
        justChanged = true;
        characterMovable = !characterMovable;
    }

    if(!characterMovable) {
        swingHook();
    }


    returnInformation[0] = characterPositionX;
    returnInformation[1] = characterPositionY;
    returnInformation[2] = resetSkySide;

    returnInformation[3] = xBeginHook;
    returnInformation[4] = yBeginHook;
    returnInformation[5] = xEndHook;
    returnInformation[6] = yEndHook;
    returnInformation[7] = drawHookBool;

    return returnInformation;
}

// void gamelogic::drawHook()
// {
//     xOrigin = characterPositionX + 25;
//     yOrigin = 81;

//     // Calculate the radius of the circle
//     radius = 15;

//     // Calculate the angle step to draw points on the circle
//     angleStep = 0.05; // Change this to adjust the spacing of points on the circle

//     // swing right to left
//     for (angle = 0; angle < 1 * PI; angle += angleStep)
//     {
//         Nunchuk.getState(NUNCHUK_ADDRESS);
//         if (Nunchuk.state.c_button == 0 && justChanged)
//         {
//             justChanged = false;
//         }

//         if (Nunchuk.state.c_button == 1 && !justChanged)
//         {
//             justChanged = true;
//             characterMovable = true;
//             break;
//         }

//         if (Nunchuk.state.z_button == 1)
//         {
//             // calculateAndDrawHook();
//         }
//         else
//         {
//             removeHook();
//         }
//     }

//     if (!characterMovable)
//     {
//         // swing left to right
//         for (angle = PI; angle > 0; angle -= angleStep)
//         {
//             Nunchuk.getState(NUNCHUK_ADDRESS);
//             if (Nunchuk.state.c_button == 0 && justChanged)
//             {
//                 justChanged = false;
//             }

//             if (Nunchuk.state.c_button == 1 && !justChanged)
//             {
//                 justChanged = true;
//                 characterMovable = true;
//                 break;
//             }

//             if (Nunchuk.state.z_button == 1)
//             {
//                 // calculateAndDrawHook();
//             }
//             else
//             {
//                 removeHook();
//             }
//         }
//     }

// }

// void gamelogic::calculateAndDrawHook()
// {
//     for (int i = 15; i < 400; i++)
//     {
//         // Calculate the coordinates on the circle using polar coordinates
//         xCircle = xOrigin + (int)(i * cos(angle));
//         yCircle = yOrigin + (int)(i * sin(angle));

//         // Draw lines from origin to points on the circle
//         c.drawHook(xOrigin, yOrigin, xCircle, yCircle);
//         _delay_ms(5);

//         for (int j = 0; j < 9; j++)
//         {
//             Item &item = items[j];
//             if ((xCircle > item.x && xCircle < (item.x + item.size)) && (yCircle > item.y && yCircle < (item.y + item.size)))
//             {
//                 turnAround = true;
//                 int xStopMoment = xCircle;
//                 int yStopMoment = yCircle;

//                 c.removeItem(item.x, item.y, item.size);
//                     item.x = 0;
//                     item.y = 0;

//                 for (int i2 = 0; i2 < i - (15 + item.size/2); i2++)
//                 {
//                     int xCircle = xStopMoment - (int)(i2 * cos(angle));
//                     int yCircle = yStopMoment - (int)(i2 * sin(angle));

//                     c.drawItemWhenGrabbed(xCircle, yCircle, item.size, item.type);

//                     if(item.size < 10) {
//                         _delay_ms(3);
//                     } else if (item.size < 20) {
//                         _delay_ms(10);
//                     } else if (item.size < 30) {
//                         _delay_ms(20);
//                     } else if (item.size < 40) {
//                         _delay_ms(40);
//                     } else {
//                         _delay_ms(80);
//                     }

//                     c.removeHookSquare(xCircle, yCircle, item.size);

//                 }
//                 c.displayItemValue(item.value);
//                 c.updateScore(item.value);
//             }
//         }

//         if (yCircle == 240 || xCircle == 0 || xCircle == 320)
//         {
//             turnAround = true;
//             int xStopMoment = xCircle;
//             int yStopMoment = yCircle;
//             for (int i2 = 0; i2 < i - 15; i2++)
//             {
//                 int xCircle = xStopMoment - (int)(i2 * cos(angle));
//                 int yCircle = yStopMoment - (int)(i2 * sin(angle));

//                 c.removeHook(xStopMoment, yStopMoment, xCircle, yCircle);

//                 _delay_ms(5);
//             }
//         }
//         if (turnAround)
//         {
//             turnAround = false;
//             break;
//         }
//     }
// }

void gamelogic::swingHook()
{

    xBeginHook = characterPositionX + 25;
    yBeginHook = 81;

    xEndHook = xBeginHook + (int)(radius * cos(angle));
    yEndHook = yBeginHook + (int)(radius * sin(angle));

    // Draw lines from origin to points on the circle
    drawHookBool = false;

    if (milliSeconds - startTime > 20)
    {
        Serial.println(milliSeconds);
        drawHookBool = true;
        startTime = milliSeconds;
        angle += angleStep;
    }

    if(angle > PI) {
        angle = 0;
    }
}

void gamelogic::moveCharacter()
{
    Nunchuk.getState(NUNCHUK_ADDRESS);
    // move character left and right
    if ((Nunchuk.state.joy_x_axis < 128 && characterPositionX > 0) && characterMovable)
    {
        characterPositionX -= 5;
        resetSkySide = 0;
    }
    if ((Nunchuk.state.joy_x_axis > 128 && characterPositionX < 270) && characterMovable)
    {
        characterPositionX += 5;
        resetSkySide = 1;
    }
}