/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition des méthodes qui permettent de configurer les interruptions et le Timer 1
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "configInterruption.h"

void Interruption::INT0Setter()
{
    cli();
    
    DDRD &= (0<<PD2);

    EIMSK |= (1<<INT0);
    EICRA |= (1<<ISC00);

    sei();
}

void Interruption::timer1Start(uint16_t time, bool prescaler, uint8_t mode)
{
    cli();

    TCNT1 = 0;
    OCR1A = time;
    TCCR1A |= (1<<COM1A0);
    //precaler of 1024
    if(prescaler) {TCCR1B |= (1<<CS12) | (1<<CS10);}
    else {TCCR1B |= (1<<CS10);}
    //mode PWM, phase correct
    if(mode == 2) {TCCR1A |= (1<<WGM10);}
    //mode CTC
    else if(mode == 1) {TCCR1B |= (1<<WGM12);}
    //else mode normal
    TCCR1C = 0;
    TIMSK1 |= (1<<OCIE1A);

    sei();
}

void Interruption::delay(uint16_t time)
{
    //time in ms
    uint16_t tick = (8000/1024)*time;
    Interruption::timer1Start(tick , true, 1);
}

void Interruption::timer1Stop()
{
    TIMSK1 &= (0 << OCIE1A);
}
