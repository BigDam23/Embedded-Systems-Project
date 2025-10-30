/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
Section : 01

TP4 PB2

Description: 
*/

#define F_CPU 8000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define GREEN ((0 << PA0) |(1 << PA1))
#define RED ((1 << PA0) |(0 << PA1))
#define OFF ((0 << PA0) |(0 << PA1))

volatile uint8_t gMinuterieExpiree;
volatile uint8_t gBoutonPoussoir;

ISR (TIMER1_COMPA_vect) {
gMinuterieExpiree = 1;
}

ISR (INT0_vect) {
// anti-rebond
 _delay_ms(30);
if (PIND & (1 << PD2)) {  // Si le bouton est pressé
    gBoutonPoussoir = 1;
}
EIFR |= (1 << INTF0);  // Réinitialiser l’interruption
}

void partirMinuterie ( uint16_t duree ) {
gMinuterieExpiree = 0;
// mode CTC du timer 1 avec horloge divisée par 1024
// interruption après la durée spécifiée
TCNT1 = 0 ;
OCR1A = duree;
TCCR1A = 0;
TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); ;
TCCR1C = 0;
TIMSK1 = (1 << OCIE1A);
}

void initialisation(void) {
    cli();
    DDRA = (1 << PA0) | (1 << PA1);
    DDRD = (0 << PD2);
    EIMSK |= (1 << INT0);
    sei();
}

int main(void) {
    initialisation();

    while (1) {
        // Attente initiale de 10 secondes
        _delay_ms(10000);

        // Clignotement rapide de la DEL rouge (1/10 s)
        PORTA = RED;
        _delay_ms(100);
        PORTA = OFF;

        // Lancer la minuterie pour 1 seconde (8000000 / 1024 ≈ 7812 cycles)
        partirMinuterie(7812);
        EIMSK |= (1 << INT0);
        EICRA |= (1 << ISC00) | (1 << ISC01);  // Interruption sur front montant

        // Attendre une interruption : bouton ou minuterie
        do {
        } while (gMinuterieExpiree == 0 && gBoutonPoussoir == 0);

        cli();  // Désactiver les interruptions

        // Vérification de la réponse du joueur
        if (gBoutonPoussoir) {
            PORTA = GREEN;  // Réaction rapide -> DEL verte
        } else {
            PORTA = RED;  // Trop lent ou aucune réponse -> DEL rouge
        }

        // Fin du jeu : attendre RESET pour recommencer
        while (1);
    }

    return 0;
}
