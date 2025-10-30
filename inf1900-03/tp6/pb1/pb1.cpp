/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
Section : 01

TP6 PB1

Description: Programme qui, quand le bouton est enfoncé, un compteur qui incrémente 10 fois par seconde est activé.
Quand le bouton est relâché ou lorsque le compteur atteint 120, la lumière clignote vert pendant 1/2 seconde.
Ensuite, la carte mère ne fait rien. Puis, deux secondes plus tard, la lumière rouge s'allume.
Elle devra clignoter (compteur / 2) fois au rythme de 2 fois par seconde. Ensuite, la lumière tourne au vert pendant une seconde.
Finalement, elle s'éteint et le robot revient à son état original.

En vert : B1 = 1 et B0 = 0
En rouge : B1 = 0 et B0 = 1
La branche positive est associee a B0 et la branche negative a B1.
Le bouton poussoir est associee au port D2, sans le cavalier Inten, il est place sur un breadboard.
*/
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define GREEN ((0 << PB0) |(1 << PB1))
#define RED ((1 << PB0) |(0 << PB1))
#define OFF ((0 << PB0) |(0 << PB1))

volatile bool buttonPressed = false;
volatile uint8_t counter = 0;

const uint8_t MAX_COUNTER = 120;

const uint16_t TIMER_100MS = (8000000/1024)/10;
const uint16_t TIMER_2000MS = 2000;
const uint16_t TIMER_1000MS = 1000;
const uint16_t TIMER_500MS = 500;



ISR (INT0_vect) {
    _delay_ms(30);
    if (buttonPressed) {
        buttonPressed = false;
    } 
    else {
        buttonPressed = true;
    }
}

ISR (TIMER1_COMPA_vect) {
    counter++;
}

void startTimer ( uint16_t timer ) {
    TCNT1 = 0 ;
    OCR1A = timer;
    TCCR1A |= (1<<COM1A0);
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    TCCR1C = 0;
    TIMSK1 |= (1 << OCIE1A);
}

void initialisation(void) {
    cli();
    DDRB = (1 << PB0) | (1 << PB1);
    DDRD &= (0<<PD2);
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00);
    EICRA &= ~(1 << ISC01);
    sei();
}

void blinkRed(uint8_t blinks) {
    for (uint8_t i = 0; i < blinks; i++) {
        PORTA = RED;
        _delay_ms(TIMER_250MS);
        PORTA = OFF;
        _delay_ms(TIMER_250MS);
    }
}


int main(void) {
    initialisation();
    while(true){
        while(!buttonPressed) {}
        counter = 0;
        startTimer(TIMER_100MS);
        while ((counter < MAX_COUNTER) && buttonPressed) {}

        TIMSK1 &= ~(1 << OCIE1A);

        PORTA = GREEN;
        _delay_ms(TIMER_500MS);
        PORTA = OFF;

        _delay_ms(TIMER_2000MS);

        blinkRed(counter/2);

        PORTA = GREEN;
        _delay_ms(TIMER_1000MS);
        PORTA = OFF;
    }
    return 0;
}