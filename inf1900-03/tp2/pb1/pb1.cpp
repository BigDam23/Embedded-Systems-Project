/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
Section : 01

TP2 PB1

Description: Programme qui allume une DEL a la couleur verte pendant 2 secondes lorsque l'usager pese et relache 3 fois un bouton-poussoir, sinon la DEL est toujours eteinte.

En vert : A1 = 1 et A0 = 0
La branche positive est associee a A0 et la branche negative a A1.
Le bouton poussoir est associee au port D2.

Tableau d'etat:
+--------------------+----+----------------+----+----+
| État présent       | D2 | État suivant   | A1 | A0 |
+--------------------+----+----------------+----+----+
| INIT               | 0  | INIT           | 0  | 0  |
| INIT               | 1  | FirstPress     | 0  | 0  |
| FirstPress         | 0  | FirstRelease   | 0  | 0  |
| FirstPress         | 1  | FirstPress     | 0  | 0  |
| FirstRelease       | 0  | FirstRelease   | 0  | 0  |
| FirstRelease       | 1  | SecondPress    | 0  | 0  |
| SecondPress        | 0  | SecondRelease  | 0  | 0  |
| SecondPress        | 1  | SecondPress    | 0  | 0  |
| SecondRelease      | 0  | SecondRelease  | 0  | 0  |
| SecondRelease      | 1  | ThirdPress     | 0  | 0  |
| ThirdPress         | 0  | ThirdRelease   | 0  | 0  |
| ThirdPress         | 1  | ThirdPress     | 0  | 0  |
| ThirdRelease       | 0  | INIT           | 1  | 0  |
| ThirdRelease       | 1  | INIT           | 1  | 0  |
+--------------------+----+----------------+----+----+
*/
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>


#define GREEN ((0 << PA0) |(1 << PA1))
#define OFF ((0 << PA0) |(0 << PA1))

enum State { INIT, firstPress, firstRelease, secondPress, secondRelease, thirdPress, thirdRelease };
State currentState = INIT;

bool buttonPressed = false;

void nextState() {
    switch (currentState) {
        case INIT:
            currentState = firstPress;
            break;
        case firstPress:
            currentState = firstRelease;
            break;
        case firstRelease:
            currentState = secondPress;
            break;
        case secondPress:
            currentState = secondRelease;
            break;
        case secondRelease:
            currentState = thirdPress;
            break;
        case thirdPress:
            currentState = thirdRelease;
            break;
        case thirdRelease:
            currentState = INIT;
            break;
            
    }
}

void pressButton(){
    if (PIND & 0x04 && !buttonPressed) {
        buttonPressed = true;
        nextState();
    }
}

void releaseButton(){
    if (!(PIND & 0x04) && buttonPressed) {
        buttonPressed = false;
        nextState();
    }
}

void performStateAction() {
    switch (currentState) {
        case INIT:
            PORTA = OFF;
            pressButton();
            break;
        case firstPress:
            releaseButton();
            break;
        case firstRelease:
            pressButton();
            break;
        case secondPress:
            releaseButton();
            break;
        case secondRelease:
            pressButton();
            break;
        case thirdPress:
            releaseButton();
            break;
        case thirdRelease:
            PORTA = GREEN;
            _delay_ms(2000);
            nextState();
    }
}

int main(void) {
    DDRA = (1 << PA0) | (1 << PA1);
    DDRD &= ~(1 << PD2);
    while (1)
    {
        performStateAction();
    }
    return 0;
}