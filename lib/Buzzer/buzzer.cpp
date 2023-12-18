#include "buzzer.h"
#include "arduino.h"
//G2 - G3
int notes[] = {120, 129, 138, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247, 262};

ISR(TIMER2_COMPA_vect){}

buzzer::buzzer(){
    timer2_init();
}

void timer2_init(void)
{
    // Set PD3 as output
    DDRD |= (1 << DDD3);

    // Set Timer2 to CTC mode
    TCCR2A |= (1 << WGM21);
    TCCR2B &= ~(1 << WGM22);

    // Toggle OC2B on compare match (PD3 is OC2B)
    TCCR2A |= (1 << COM2B0);
    TCCR2A &= ~(1 << COM2B1);

    // Set prescaler to 8
    TCCR2B |= (1 << CS22);
    TCCR2B &= ~((1 << CS20) | (1 << CS21));

    // Set the compare match value
    OCR2A = notes[0];

    // Enable the compare match interrupt
    TIMSK2 |= (1 << OCIE2A);
}