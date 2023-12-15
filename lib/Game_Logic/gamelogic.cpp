#include "gamelogic.h"

gamelogic::gamelogic() {}

void gamelogic::getTime(uint16_t time) {
    timePassed += time;
    Serial.println(timePassed);
}