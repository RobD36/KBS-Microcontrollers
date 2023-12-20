#include "gamelogic.h"

int score = 0;

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
    // gametick function calculates all the game logic and returns the information to the main file

    // debounce c & z button
    Nunchuk.getState(NUNCHUK_ADDRESS);
    if (Nunchuk.state.c_button == 0 && justChangedC)
    {
        justChangedC = false;
    }
    // move character calculations
    moveCharacter();
    // hook calculations
    hookLogic(itemsArray);

    // return information to main file
    returnInformation[0] = characterPositionX;
    returnInformation[1] = characterPositionY;
    returnInformation[2] = resetSkySide;

    returnInformation[3] = xBeginHook;
    returnInformation[4] = yBeginHook;
    returnInformation[5] = xEndHook;
    returnInformation[6] = yEndHook;
    returnInformation[7] = hookSwinging;
    returnInformation[8] = deleteHook;

    returnInformation[9] = withdrawHook;
    returnInformation[10] = xBeginRemoveHook;
    returnInformation[11] = yBeginRemoveHook;
    returnInformation[12] = xEndRemoveHook;
    returnInformation[13] = yEndRemoveHook;

    returnInformation[14] = itemGrabbedBool;
    returnInformation[15] = itemGrabbed;

    returnInformation[16] = scoreHasChanged;

    if(scoreHasChanged) {
        scoreHasChanged = false;
    }

    return returnInformation;
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
    else if ((Nunchuk.state.joy_x_axis > 128 && characterPositionX < 270) && characterMovable)
    {
        characterPositionX += 5;
        resetSkySide = 1;
    }
    else
    {
        resetSkySide = 2;
    }
}

void gamelogic::hookLogic(Item items[])
{
    Nunchuk.getState(NUNCHUK_ADDRESS);

    // switch character mode, moving and swinging hook
    if (Nunchuk.state.c_button == 1 && !justChangedC && !throwHookBool)
    {
        justChangedC = true;
        characterMovable = !characterMovable;
        deleteHook = characterMovable;
    }

    // if character is not movable, thus swinging hook mode
    if (!characterMovable)
    {
        // check z button for throwing hook
        Nunchuk.getState(NUNCHUK_ADDRESS);

        if (Nunchuk.state.z_button == 1 && !justChangedZ && !throwHookBool && !withdrawHook)
        {
            justChangedZ = true;
            throwHookBool = !throwHookBool;
        }

        // throw hook if conditions are met, otherwise continue to swing hook. Swing hook is default
        if (throwHookBool)
        {
            hookSwinging = true; // drawhook boolean used for display function
            throwHook(items);    // Fix: Remove the '&' before sizeOfArray
            justChangedZ = false;
        }
        else
        {
            swingHook();
        }
    }

    else
    {
        // if character is switched to move mode, reset swing hook angle and the drawhook boolean for display function is set to false
        hookSwinging = false;
        angle = 0;
    }
}

void gamelogic::swingHook()
{

    xBeginHook = characterPositionX + 25;
    yBeginHook = 81;

    xEndHook = xBeginHook + (int)(radius * cos(angle));
    yEndHook = yBeginHook + (int)(radius * sin(angle));

    // Draw lines from origin to points on the circle
    hookSwinging = false;

    if (milliSeconds - startTime > 0)
    {
        hookSwinging = true;
        startTime = milliSeconds;

        if (angle < PI && swingDirection)
        {
            angle += angleStep;
        }
        else if (angle > 0 && !swingDirection)
        {
            angle -= angleStep;
        }
        else
        {
            swingDirection = !swingDirection;
        }
    }
}

void gamelogic::throwHook(Item items[])
{
    if (throwDirectionDown)
    { // throw hook out

        // Calculate the coordinates on the circle using polar coordinates
        xEndHook = xBeginHook + (int)(hookCounterSteps * cos(angle));
        yEndHook = yBeginHook + (int)(hookCounterSteps * sin(angle));
        hookCounterSteps += 2;

        for (int j = 0; j < sizeOfItemArray; j++)
        {
            Item &item = items[j];
            if ((xEndHook > item.x && xEndHook < (item.x + item.size)) && (yEndHook > item.y && yEndHook < (item.y + item.size)))
            {
                itemGrabbedBool = true;
                itemGrabbed = j;
                currentGrabbedItem = &item;

                throwDirectionDown = false;
                stepsTaken = hookCounterSteps;
                hookCounterSteps = 15;

                // for (int i2 = 0; i2 < i - (15 + item.size / 2); i2++)
                // {
                //     // int xCircle = xStopMoment - (int)(i2 * cos(angle));
                //     // int yCircle = yStopMoment - (int)(i2 * sin(angle));

                //     c.drawItemWhenGrabbed(xCircle, yCircle, item.size, item.type);

                //     c.removeHookSquare(xCircle, yCircle, item.size);
                // }
            }
        }

        if (yEndHook > 240 || xEndHook < 0 || xEndHook > 320)
        {
            throwDirectionDown = false;
            // throwHookBool = false;
            stepsTaken = hookCounterSteps;
            hookCounterSteps = 15;
        }
    }
    else if (!throwDirectionDown && !itemGrabbedBool)
    { // pull hook back in without item
        withdrawHook = true;
        hookSwinging = false;
        xBeginRemoveHook = xEndHook;
        yBeginRemoveHook = yEndHook;

        xEndRemoveHook = xBeginRemoveHook - (int)(removeHookCounterSteps * cos(angle));
        yEndRemoveHook = yBeginRemoveHook - (int)(removeHookCounterSteps * sin(angle));

        removeHookCounterSteps += 2;

        if (removeHookCounterSteps > stepsTaken - 15)
        { // reached begin point
            throwDirectionDown = true;
            throwHookBool = false;
            withdrawHook = false;
            removeHookCounterSteps = 0;
        }
    }

    else if (!throwDirectionDown && itemGrabbedBool)
    { // pull hook back in with item
        withdrawHook = true;
        hookSwinging = false;
        xBeginRemoveHook = xEndHook;
        yBeginRemoveHook = yEndHook;

        xEndRemoveHook = xBeginRemoveHook - (int)(removeHookCounterSteps * cos(angle));
        yEndRemoveHook = yBeginRemoveHook - (int)(removeHookCounterSteps * sin(angle));

        currentGrabbedItem->x = xEndRemoveHook - (currentGrabbedItem->size / 2);
        currentGrabbedItem->y = yEndRemoveHook - (currentGrabbedItem->size / 2);
        

        removeHookCounterSteps += 2;

        if (removeHookCounterSteps > stepsTaken - (15 + currentGrabbedItem->size / 2))
        { // reached begin point
            updateScore();

            delete currentGrabbedItem;
            currentGrabbedItem = nullptr;
            //switches last item of array to position of item grabbed
            if (itemGrabbed != sizeOfItemArray - 1)
            {
                items[itemGrabbed] = items[sizeOfItemArray - 1];
            }
            sizeOfItemArray--;

            // itemGrabbed = NULL; // item index in array
            itemGrabbedBool = false;

            throwDirectionDown = true;
            throwHookBool = false;
            withdrawHook = false;
            removeHookCounterSteps = 0;
        }
    }
}

void gamelogic::updateScore() {
    score += currentGrabbedItem->value;
    scoreHasChanged = true;
}