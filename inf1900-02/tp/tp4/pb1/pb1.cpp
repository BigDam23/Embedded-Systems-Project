#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
using namespace std;

enum class Color {RED, AMBER, GREEN, OFF};

const uint8_t DELAY_AMBER = 5;
const uint8_t RED_LIGHT = 0x01;
const uint8_t GREEN_LIGHT = 0x02;
const uint8_t OFF_LIGHT = 0xFC;

volatile Color gColor = Color::RED;

void initialisation()
{
    cli();

    DDRA |= (1<<PA0) | (1<<PA1);
    DDRD &= (0<<PD2);

    EIMSK |= (1<<INT0);
    EICRA |= (1<<ISC00);

    sei();
}

ISR(INT0_vect)
{
    _delay_ms(30);

    switch(gColor)
    {
        case Color::RED:
            if(PIND & (1<<PD2)){gColor = Color::AMBER;}
            else{gColor = Color::OFF;}
            break;
        case Color::AMBER:
            gColor = Color::GREEN;
            break;
        case Color::GREEN:
            gColor = Color::RED;
            break;
        case Color::OFF:
            gColor = Color::GREEN;
            break;
    }

    EIFR |= (1 << INTF0) ;
}

void activateRedLight()
{
    PORTA |= RED_LIGHT;
    PORTA &= ~GREEN_LIGHT;
}

void activateGreenLight()
{
    PORTA |= GREEN_LIGHT;
    PORTA &= ~RED_LIGHT;
}

int main()
{
    initialisation();

    while(true)
    {
        switch(gColor)
        {
            case Color::RED:
                activateRedLight();
                break;
            case Color::AMBER:
                activateRedLight();
                _delay_ms(DELAY_AMBER);
                activateGreenLight();
                _delay_ms(DELAY_AMBER);
                break;
            case Color::GREEN:
                activateGreenLight();
                break;
            case Color::OFF:
                PORTA &= OFF_LIGHT;
                break;
        }
    }
}