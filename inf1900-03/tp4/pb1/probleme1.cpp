/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
Section : 01

TP4 PB1

Description: Programme qui allume une DEL en rouge a l'allumage, si le bouton poussoir est pese, la couleur ambre est affiche, si le meme bouton est relache,
la couleur devient verte, si le bouton est de nouveau pese, la DEL s'affiche en rouge et si c'est relache, la DEL s'eteint. Si le bouton est de nouveau pese,
la DEL s'allume en vert, quand il est relache, ca revient au debut du cycle, donc la DEL est rouge.

DEL rouge : A1 = 0 et A0 = 1
DEL verte : A1 = 1 et A0 = 0
DEL ambre : alternance rapide entre la DEL rouge et la DEL verte (represente par A1 = 1 et A0 = 1)
La branche positive de la DEL est associee a A0 et la branche negative de la DEL a A1.
Le bouton poussoir est associee au port D2.

Tableau d'etat: 
+---------------------+----+----------------+----+----+
| État présent        | D2 | État suivant   | A1 | A0 |
+---------------------+----+----------------+----+----+
| INIT                | 0  | INIT           | 0  | 1  |
| INIT                | 1  | FirstPress     | 0  | 1  |
| FirstPress          | 0  | FirstRelease   | 1  | 1  |
| FirstPress          | 1  | FirstPress     | 1  | 1  |
| FirstRelease        | 0  | FirstRelease   | 1  | 0  |
| FirstRelease        | 1  | SecondPress    | 1  | 0  |
| SecondPress         | 0  | SecondRelease  | 0  | 1  |
| SecondPress         | 1  | SecondPress    | 0  | 1  |
| SecondRelease       | 0  | SecondRelease  | 0  | 0  |
| SecondRelease       | 1  | ThirdPress     | 0  | 0  |
| ThirdPress          | 0  | INIT           | 1  | 0  |
| ThirdPress          | 1  | ThirdPress     | 1  | 0  |
+---------------------+----+----------------+----+----+
*/

#define F_CPU 8000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define GREEN ((0 << PA0) |(1 << PA1))
#define RED ((1 << PA0) |(0 << PA1))
#define OFF ((0 << PA0) |(0 << PA1))

volatile uint8_t currentState = 0;

bool buttonPressed = false;

// placer le bon type de signal d'interruption
// à prendre en charge en argument
ISR (INT0_vect) {
    // laisser un délai avant de confirmer la réponse du
    // bouton-poussoir: environ 30 ms (anti-rebond)
    _delay_ms (30);
    // se souvenir ici si le bouton est pressé ou relâché
    if (PIND & (1 << PD2)) {  // Bouton appuyé (front montant)
        if (!buttonPressed) { // Éviter double appui
            buttonPressed = true;
            EICRA |= (1 << ISC01);  // Détection sur front descendant (bouton relâché)
            EICRA &= ~(1 << ISC00);
        }
    } else {  // Bouton relâché (front descendant)
        if (buttonPressed) {
            buttonPressed = false;
            EICRA |= (1 << ISC00);  // Détection sur front montant (bouton pressé)
            EICRA &= ~(1 << ISC01);
        }
    }
    // changements d'état tels que ceux de la
    // semaine précédente
    switch (currentState) {
        case 0:
            currentState = 1;
            break;
        case 1:
            currentState = 2;
            break;
        case 2:
            currentState = 3;
            break;
        case 3:
            currentState = 4;
            break;
        case 4:
            currentState = 5;
            break;
        case 5:
            currentState = 0;
            break;
    }
    // Voir la note plus bas pour comprendre cette instruction et son rôle
    EIFR |= (1 << INTF0) ;
}

void initialisation (void) {
// cli est une routine qui bloque toutes les interruptions.
// Il serait bien mauvais d'être interrompu alors que
// le microcontrôleur n'est pas prêt...
cli ();
// configurer et choisir les ports pour les entrées
// et les sorties. DDRx... Initialisez bien vos variables
DDRA = (1 << PA0) | (1 << PA1);
DDRD &= ~(1 << PD2);
// cette procédure ajuste le registre EIMSK
// de l’ATmega324PA pour permettre les interruptions externes
EIMSK |= (1 << INT0) ;
// il faut sensibiliser les interruptions externes aux
// changements de niveau du bouton-poussoir
// en ajustant le registre EICRA
EICRA |= (1 << ISC01) | (1 << ISC00);
// sei permet de recevoir à nouveau des interruptions.
sei ();
}

int main(void) {
    initialisation();
    while (1)
    {
        switch (currentState) {
        case 0:
            PORTA = RED;
            break;
        case 1:
            PORTA = RED;
            _delay_ms(10);
            PORTA = GREEN;
            _delay_ms(10);
            break;
        case 2:
            PORTA = GREEN; 
            break;
        case 3:
            PORTA = RED; 
            break;
        case 4:
            PORTA = OFF; 
            break;
        case 5:
            PORTA = GREEN;
            break;
    }
    }
    return 0;
}
