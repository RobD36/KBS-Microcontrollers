#ifndef HOOK_H
#define HOOK_H

#include <util/delay.h>
#include "items.h"

class hook {
    public:
        hook();
        void calculateAndDrawHook(int x, int y, float angle, Item items[]);
        void removeHook(int x, int y, int radius, float angle);

        bool turnAround;
};

#endif
