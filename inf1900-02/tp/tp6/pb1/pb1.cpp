#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
using namespace std;

volatile bool gButtonPressed = false;
volatile uint8_t gCounter = 0;

const uint16_t TIMER_100MS = (8000000/1024)/10;
const uint8_t MAX_COUNTER = 120;

const uint16_t DELAY_500MS = 500;
const uint16_t DELAY_2000MS = 2000;
const uint8_t DELAY_250MS = 250;

const uint8_t RED_LIGHT = 0x01;
const uint8_t GREEN_LIGHT = 0x02;
const uint8_t OFF_LIGHT = 0xFC;

void initialisation()
{
    cli();

    DDRB |= (1<<PB0) | (1<<PB1);
    DDRD &= (0<<PD2);

    EIMSK |= (1<<INT0);
    EICRA |= (1<<ISC00);

    sei();
}

void beginTimer(uint16_t time) 
{
    TCNT1 = 0;
    OCR1A = time;
    TCCR1A |= (1<<COM1A0);
    TCCR1B |= (1<<CS12) | (1<<CS10) | (1<<WGM12);
    TCCR1C = 0;
    TIMSK1 |= (1<<OCIE1A);
}

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

ISR(INT0_vect)
{
    if(gButtonPressed) {gButtonPressed = false;}
    else {gButtonPressed = true;}
}

ISR(TIMER1_COMPA_vect)
{
    gCounter++;
}

int main()
{
    while (true)
    {
        initialisation();
        while(!gButtonPressed) {}
        beginTimer(TIMER_100MS);
        while(gButtonPressed && (gCounter < MAX_COUNTER)) {}
        TIMSK1 &= (0 << OCIE1A);
        activateGreenLight();
        _delay_ms(DELAY_500MS);
        PORTB &= OFF_LIGHT;
        _delay_ms(DELAY_2000MS);

        for (uint8_t i = 0; i < gCounter/2; i++)
        {
            activateRedLight();
            _delay_ms(DELAY_250MS);
            PORTB &= OFF_LIGHT;
            _delay_ms(DELAY_250MS);
        }
    }
}