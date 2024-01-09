#include "brightness.h"

brightness::brightness() {}

int brightness::getPotentiometerValue()
{
    // Set AREF to AVCC, enable ADC, and set prescaler to 128
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // Select ADC channel A0
    ADMUX &= ~(1 << MUX0 | 1 << MUX1 | 1 << MUX2 | 1 << MUX3);

    // Start single conversion
    ADCSRA |= (1 << ADSC);

    // Return ADC result
    return ADC;
}

void brightness::setBrightness(int value)
{
    int brightnessValue = getPotentiometerValue();
    int pwmValue = map(brightnessValue, 0, 1023, 0, 255);

    pwmValue = max(pwmValue, 20);
    
    // Set up Timer0 for Fast PWM mode
    TCCR0A = (1 << COM0B1) | (1 << WGM01) | (1 << WGM00); // Non-inverting mode on OC0B, Fast PWM mode
    TCCR0B = (1 << CS00); // No prescaling

    // Set the duty cycle (brightness)
    OCR0B = pwmValue; // Duty cycle (0-255)

    // Set up pin OC0B (PD5, digital pin 5 on the Arduino Uno) for output
    DDRD |= (1 << PD5);
}
