/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
Section : 01

TP6 PB2

Description: Programme qui,
Si la lumière est basse (en cachant la photorésistance), la DEL prendra la couleur verte.
Si la lumière est à un bon niveau (lumière ambiante), la DEL tournera à l'ambré.
Si la lumière est trop forte (photorésistance sous une lampe de poche), la DEL devient rouge.

En vert : B1 = 1 et B0 = 0
En rouge : B1 = 0 et B0 = 1
En ambre : Clignotement rapide entre le vert et le rouge
La branche positive est associee a B0 et la branche negative a B1.
La photoresistance est branchée au port A0.
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <can.h>

#define GREEN ((0 << PB0) |(1 << PB1))
#define RED ((1 << PB0) |(0 << PB1))
#define OFF ((0 << PB0) |(0 << PB1))

const uint8_t MASK_CAPTOR = 0x01;
const uint8_t AMBIANT_LIGHT = 175;
const uint8_t BIG_LIGHT = 230;
const uint8_t AMBER_DELAY = 10;

int main()
{
    DDRB = (1 << PB0) | (1 << PB1);
    DDRA &= ~MASK_CAPTOR;

    can can;
    uint16_t ReadingCaptor16Bits;
    uint8_t ReadingCaptor8Bits;

    while (true)
    {
        ReadingCaptor16Bits = can.lecture(0);
        ReadingCaptor8Bits = (ReadingCaptor16Bits >> 2);

        if (ReadingCaptor8Bits < AMBIANT_LIGHT)
        {
            PORTB = GREEN;
        } 
        else if (ReadingCaptor8Bits < BIG_LIGHT)
        {
            PORTB = RED;
            _delay_ms(AMBER_DELAY);
            PORTB = GREEN;
            _delay_ms(AMBER_DELAY);
        }
        else
        {
            PORTB = RED;
        }       
    }
}
