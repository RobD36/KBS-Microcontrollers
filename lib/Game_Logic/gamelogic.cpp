#include "gamelogic.h"
#include "buzzer.h"

// initialize shared variables
int currentScore = 0;
bool displayItemValueBool = false;

gamelogic::gamelogic() {}

int *gamelogic::gameTick(Item itemsArray[], long ms, long s)
{
    // gametick function calculates all the game logic and returns the information to the main file

    // set time
    milliSeconds = ms;
    seconds = s;

    // debounce c & z button
    Nunchuk.getState(NUNCHUK_ADDRESS);
    if (Nunchuk.state.c_button == 0 && justChangedC)
    {
        justChangedC = false;
    }

    // move rats
    if (milliSeconds - updateRatsTime > 75)
    {
        updateRatsTime = milliSeconds;
        moveRats(itemsArray);
    }

    // switch character mode, moving and swinging hook
    if (Nunchuk.state.c_button == 1 && !justChangedC && !throwingHook)
    {
        justChangedC = true;
        characterMovable = !characterMovable;
        deleteHook = characterMovable;
    }

    if (characterMovable)
    {
        // move character calculations
        moveCharacter();
        // if character is switched to move mode, reset swing hook angle and the drawhook boolean for display function is set to false
        hookSwinging = false;
        angle = 0;
    }
    else
    {
        // do not redraw character when the hook is swinging
        redrawCharacter = false;
        // hook calculations
        hookLogic(itemsArray);
    }

    if (drawCharacterFirstTime)
    {
        // draw character on very first frame
        redrawCharacter = true;
        drawCharacterFirstTime = false;
    }

    saveGamelogicData();
    scoreHasChanged = false;

    // return information to main
    return returnInformation;
}

void gamelogic::saveGamelogicData()
{
    // save variables to return array
    returnInformation[CHARACTER_POSITION_X] = characterPositionX;
    returnInformation[CHARACTER_POSITION_Y] = characterPositionY;
    returnInformation[RESET_SKY_SIDE] = resetSkySide;

    returnInformation[X_BEGIN_HOOK] = xBeginHook;
    returnInformation[Y_BEGIN_HOOK] = yBeginHook;
    returnInformation[X_END_HOOK] = xEndHook;
    returnInformation[Y_END_HOOK] = yEndHook;
    returnInformation[IS_HOOK_SWINGING] = hookSwinging;
    returnInformation[DELETE_HOOK] = deleteHook;

    returnInformation[WITHDRAW_HOOK] = withdrawingHook;
    returnInformation[X_BEGIN_REMOVE_HOOK] = xBeginRemoveHook;
    returnInformation[Y_BEGIN_REMOVE_HOOK] = yBeginRemoveHook;
    returnInformation[X_END_REMOVE_HOOK] = xEndRemoveHook;
    returnInformation[Y_END_REMOVE_HOOK] = yEndRemoveHook;

    returnInformation[ITEM_GRABBED_BOOL] = itemGrabbedBool;
    returnInformation[ITEM_GRABBED_ID] = itemGrabbed;

    returnInformation[SCORE_HAS_CHANGED] = scoreHasChanged;

    returnInformation[ITEM_VALUE] = itemValue;

    returnInformation[REDRAW_CHARACTER] = redrawCharacter;
}
void gamelogic::resetVariables()
{
    displayItemValueBool = false; // Reset all variables for a new round

    // draw character at start position
    characterPositionX = 0;
    characterPositionY = 55;

    resetSkySide = 2; // don't reset sky
    xBeginHook = 0;   // begin hook at character

    // length of hooksteps throwing down reset
    xEndHook = 0;
    yEndHook = 0;

    throwDirectionDown = true;  // resetting direction of hook
    hookCounterSteps = 15;      // length of hook
    removeHookCounterSteps = 0; // starts removing hook at the end of the line
    withdrawingHook = false;    // reset hook to not being withdrawn

    itemGrabbedBool = false;       // grabbing item reset
    scoreHasChanged = false;       // score display reset
    redrawCharacter = false;       // don't redraw character
    drawCharacterFirstTime = true; // draw character at start
    characterMovable = true;       // character starts in movable mode
    throwingHook = false;          // making sure you can go in swing mode again

    itemGrabbed = 100;
}

void gamelogic::moveCharacter()
{
    Nunchuk.getState(NUNCHUK_ADDRESS);
    // move character left and right
    if ((Nunchuk.state.joy_x_axis < 128 && characterPositionX > 0) && characterMovable)
    {
        characterPositionX -= 5;
        resetSkySide = 0;
        redrawCharacter = true;
    }
    else if ((Nunchuk.state.joy_x_axis > 128 && characterPositionX < 270) && characterMovable)
    {
        characterPositionX += 5;
        resetSkySide = 1;
        redrawCharacter = true;
    }
    else
    {
        resetSkySide = 2;
        redrawCharacter = false;
    }
}

void gamelogic::hookLogic(Item items[])
{
    // if character is not movable, thus swinging hook mode
    if (!characterMovable)
    {
        // check z button for throwing hook
        Nunchuk.getState(NUNCHUK_ADDRESS);

        if (Nunchuk.state.z_button == 1 && !justChangedZ && !throwingHook && !withdrawingHook)
        {
            justChangedZ = true;
            throwingHook = !throwingHook;
        }

        // throw hook if conditions are met, otherwise continue to swing hook. Swing hook is default
        if (throwingHook)
        {
            hookSwinging = true; // drawhook boolean used for display function
            throwHook(items);
            justChangedZ = false;
        }
        else
        {
            swingHook();
        }
    }
}

void gamelogic::swingHook()
{

    xBeginHook = characterPositionX + 25;

    xEndHook = xBeginHook + (int)(radius * cos(angle));
    yEndHook = yBeginHook + (int)(radius * sin(angle));

    // Draw lines from origin to points on the circle
    hookSwinging = false;

    if (milliSeconds - swingTime > 0)
    {
        hookSwinging = true;
        swingTime = milliSeconds;

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
        throwHookDown(items);
    }
    else if (!throwDirectionDown && !itemGrabbedBool)
    {
        withdrawHookWithoutItem();
    }

    else if (!throwDirectionDown && itemGrabbedBool)
    {
        withdrawHookWithItem(items);
    }
}

void gamelogic::withdrawHookWithItem(Item items[])
{
    // pull hook back in with item
    withdrawingHook = true;
    hookSwinging = false;
    xBeginRemoveHook = xEndHook;
    yBeginRemoveHook = yEndHook;

    xEndRemoveHook = xBeginRemoveHook - (int)(removeHookCounterSteps * cos(angle));
    yEndRemoveHook = yBeginRemoveHook - (int)(removeHookCounterSteps * sin(angle));

    if (currentGrabbedItem->type == RAT)
    {
        currentGrabbedItem->x = xEndRemoveHook - 10;
        currentGrabbedItem->y = yEndRemoveHook - 6;
    }
    else
    {
        currentGrabbedItem->x = xEndRemoveHook - (currentGrabbedItem->size / 2);
        currentGrabbedItem->y = yEndRemoveHook - (currentGrabbedItem->size / 2);
    }

    removeHookCounterSteps += steps;

    if (removeHookCounterSteps > stepsTaken - (15 + currentGrabbedItem->size / 2))
    { // reached begin point
        itemValue = currentGrabbedItem->value;
        updateScore();
        buzzer::playPickupsound();

        // switches last item of array to position of item grabbed
        if (itemGrabbed != sizeOfItemArray)
        {
            items[itemGrabbed] = items[sizeOfItemArray - 1];
        }
        sizeOfItemArray--;
        itemGrabbed = 100;

        itemGrabbedBool = false;

        throwDirectionDown = true;
        throwingHook = false;
        withdrawingHook = false;
        removeHookCounterSteps = 0;
    }
}

void gamelogic::withdrawHookWithoutItem()
{
    // pull hook back in without item
    withdrawingHook = true;
    hookSwinging = false;
    xBeginRemoveHook = xEndHook;
    yBeginRemoveHook = yEndHook;

    xEndRemoveHook = xBeginRemoveHook - (int)(removeHookCounterSteps * cos(angle));
    yEndRemoveHook = yBeginRemoveHook - (int)(removeHookCounterSteps * sin(angle));

    removeHookCounterSteps += steps;

    if (removeHookCounterSteps > stepsTaken - 15)
    { // reached begin point
        throwDirectionDown = true;
        throwingHook = false;
        withdrawingHook = false;
        removeHookCounterSteps = 0;
    }
}

void gamelogic::throwHookDown(Item items[])
{

    // Calculate the coordinates on the circle using polar coordinates
    xEndHook = xBeginHook + (int)(hookCounterSteps * cos(angle));
    yEndHook = yBeginHook + (int)(hookCounterSteps * sin(angle));
    hookCounterSteps += steps;

    for (int j = 0; j < sizeOfItemArray; j++)
    {
        Item &item = items[j];
        if (item.type == RAT)
        {
            if ((xEndHook > item.x && xEndHook < (item.x + 16)) && (yEndHook > item.y && yEndHook < (item.y + 12)))
            {
                itemGrabbedBool = true;
                itemGrabbed = j;
                currentGrabbedItem = &item;

                throwDirectionDown = false;
                stepsTaken = hookCounterSteps;
                hookCounterSteps = 15;
            }
        }
        else
        {
            if ((xEndHook > item.x && xEndHook < (item.x + item.size)) && (yEndHook > item.y && yEndHook < (item.y + item.size)))
            {
                itemGrabbedBool = true;
                itemGrabbed = j;
                currentGrabbedItem = &item;

                throwDirectionDown = false;
                stepsTaken = hookCounterSteps;
                hookCounterSteps = 15;
            }
        }
    }

    if (yEndHook > 240 || xEndHook < 0 || xEndHook > 320)
    {
        throwDirectionDown = false;
        stepsTaken = hookCounterSteps;
        hookCounterSteps = 15;
    }
}

void gamelogic::updateScore()
{
    currentScore += currentGrabbedItem->value;
    scoreHasChanged = true;
}

bool gamelogic::checkEndOfRound(int seconds, int startTimeRound)
{
    return (roundDuration - (seconds - startTimeRound) <= 0);
}

void gamelogic::moveRats(Item items[])
{
    for (int i = 0; i < sizeOfItemArray; i++)
    {
        if (items[i].type == RAT)
        {
            // Check if the rat is about to encounter any item other than rats
            bool encounterOtherItem = false;
            for (int j = 0; j < sizeOfItemArray; j++)
            {
                if (items[j].type != RAT && j != itemGrabbed)
                {
                    if ((items[i].x + 21 >= items[j].x && items[i].x - 5 <= items[j].x + items[j].size) &&
                        (items[i].y + 20 >= items[j].y && items[i].y - 4 <= items[j].y + items[j].size) ||
                        (items[i].x + 21 >= 320 || items[i].x - 5 <= 0))
                    {
                        encounterOtherItem = true;
                        break;
                    }
                }
            }

            // Change rat size to opposite if encountering other item
            if (encounterOtherItem && i != itemGrabbed)
            {
                if (items[i].size == 1)
                {
                    items[i].size = 0;
                }
                else
                {
                    items[i].size = 1;
                }
            }

            if (items[i].size == 0)
            {
                items[i].x += 3;
            }
            else if (items[i].size == 1)
            {
                items[i].x -= 3;
            }
        }
    }
}
