#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <can.h>
using namespace std;

const uint8_t DELAY_AMBER = 5;
const uint8_t RED_LIGHT = 0x01;
const uint8_t GREEN_LIGHT = 0x02;
const uint8_t OFF_LIGHT = 0xFC;

const uint8_t MASK_CAPTOR = 0x01;

const uint8_t LOW_LIGHT = 175;
const uint8_t HIGH_LIGHT = 230;

void activateRedLight()
{
    PORTB |= RED_LIGHT;
    PORTB &= ~GREEN_LIGHT;
}

void activateGreenLight()
{
    PORTB |= GREEN_LIGHT;
    PORTB &= ~RED_LIGHT;
}

int main()
{
    DDRB |= (1<<PB0) | (1<<PB1);
    DDRA &= (0<<PA0);

    can can;
    uint16_t ReadingCaptor16Bits;
    uint8_t ReadingCaptor8Bits;

    while (true)
    {
        ReadingCaptor16Bits = can.lecture(PINA & MASK_CAPTOR);
        ReadingCaptor8Bits = (ReadingCaptor16Bits >> 2);

        if (ReadingCaptor8Bits < LOW_LIGHT){activateGreenLight();} 
        else if (ReadingCaptor8Bits < HIGH_LIGHT)
        {
            activateRedLight();
            _delay_ms(DELAY_AMBER);
            activateGreenLight();
            _delay_ms(DELAY_AMBER);
        }
        else {activateRedLight();}       
    }
}