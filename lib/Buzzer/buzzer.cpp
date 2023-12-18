#include "buzzer.h"
#include "arduino.h"
/*G2 - G3
120 Hz: G2
129 Hz: G#2 or Ab2
138 Hz: A2
147 Hz: A#2 or Bb2
156 Hz: B2
165 Hz: C3
175 Hz: C#3 or Db3
185 Hz: D3
196 Hz: D#3 or Eb3
208 Hz: E3
220 Hz: F3
233 Hz: F#3 or Gb3
247 Hz: G3
262 Hz: G#3 or Ab3
*/ 
int notes[] = {262, 247, 233, 220, 208, 196, 185, 175, 165, 156, 147, 138, 129, 120, 0};

ISR(TIMER2_COMPA_vect){}

buzzer::buzzer(){
    timer2_init();
}

void buzzer::timer2_init(void)
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