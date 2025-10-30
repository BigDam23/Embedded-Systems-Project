#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
using namespace std;

volatile uint8_t gTimerExpired;
volatile uint8_t gButtonPressed ;

const uint8_t RED_LIGHT = 0x01;
const uint8_t GREEN_LIGHT = 0x02;
const uint8_t OFF_LIGHT = 0xFC;

ISR(TIMER1_COMPA_vect)
{
    gTimerExpired = 1;
}

ISR(INT0_vect) 
{
    _delay_ms(10);
    if(PIND & (1<<PD2))
    {
        gButtonPressed = 1;
    }
}

void beginTimer(uint16_t time) 
{
    gTimerExpired = 0;
    // mode CTC du timer 1 avec horloge divisée par 1024
    // interruption après la durée spécifiée
    TCNT1 = 0;
    OCR1A = time;
    TCCR1A |= (1<<COM1A0);
    TCCR1B |= (1<<CS12) | (1<<CS10) | (1<<WGM12);
    TCCR1C = 0;
    TIMSK1 |= (1<<OCIE1A);
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
void offLight()
{
    PORTA &= OFF_LIGHT;
}

void initialisation()
{
    cli();

    DDRA |= (1<<PA0) | (1<<PA1);
    DDRD &= (0<<PD2);

    EIMSK |= (1<<INT0);
    EICRA |= (1<<ISC00);

    _delay_ms(10000);
    activateRedLight();
    _delay_ms(100);
    offLight();
    beginTimer(7812);

    sei();
}

int main()
{
    initialisation();
    do {
        // attendre qu'une des deux variables soit modifiée
        // par une ou l'autre des interruptions.
    } while ( gTimerExpired == 0 && gButtonPressed == 0 );

    // Une interruption s'est produite. Arrêter toute
    // forme d'interruption. Une seule réponse suffit.
    cli ();
    if (gButtonPressed == 1) 
    {activateGreenLight();} 
    else
    {activateRedLight();}
}