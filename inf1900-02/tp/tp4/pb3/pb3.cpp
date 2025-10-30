#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
using namespace std;

void ajustementPwm (uint8_t value) 
{
    // mise à un des sorties OC1A et OC1B sur comparaison
    // réussie en mode PWM 8 bits, phase correcte
    // et valeur de TOP fixe à 0xFF (mode #1 de la table 16-5
    // page 130 de la description technique du ATmega324PA)
    OCR1A = value;
    OCR1B = value;

    // division d'horloge par 8 - implique une fréquence de PWM fixe
    TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM11);
    TCCR1B |= (1<<CS11);
    TCCR1C = 0;
}

int main()
{
    DDRD |= (1<<PD4) | (1<<PD5);
    ajustementPwm(255);
}