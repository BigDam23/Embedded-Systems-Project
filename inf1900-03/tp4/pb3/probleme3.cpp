/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
Section : 01

TP4 PB3

Description: 
*/

#define F_CPU 8000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

void ajustementPwm (uint8_t rapportA, uint8_t rapportB) {
    // mise à un des sorties OC1A et OC1B sur comparaison
    // réussie en mode PWM 8 bits, phase correcte
    // et valeur de TOP fixe à 0xFF (mode #1 de la table 16-5
    // page 130 de la description technique du ATmega324PA)
    OCR1A = rapportA;
    OCR1B = rapportB;
    // division d'horloge par 8 - implique une fréquence de PWM fixe
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
    TCCR1B = (1 << CS11); // Division d'horloge par 8
    TCCR1C = 0;
}

int main(void) {
    // Configuration des broches OC1A (PD5) et OC1B (PD4) en sortie
    DDRD |= (1 << PD5) | (1 << PD4);
    
    while (1) {
        ajustementPwm(0, 0);     // 0% PWM
        _delay_ms(2000);
        ajustementPwm(64, 64);   // 25% PWM
        _delay_ms(2000);
        ajustementPwm(128, 128); // 50% PWM
        _delay_ms(2000);
        ajustementPwm(192, 192); // 75% PWM
        _delay_ms(2000);
        ajustementPwm(255, 255); // 100% PWM
        _delay_ms(2000);
    }
}
