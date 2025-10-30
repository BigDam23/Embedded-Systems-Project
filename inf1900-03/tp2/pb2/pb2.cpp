/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
Section : 01

TP2 PB2

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
#include <avr/io.h>
#include <util/delay.h>

#define GREEN ((0 << PA0) |(1 << PA1))
#define RED ((1 << PA0) |(0 << PA1))
#define OFF ((0 << PA0) |(0 << PA1))

enum State {INIT, firstPress, firstRelease, secondPress, thirdPress, secondRelease};
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
            PORTA = RED;
            pressButton();
            break;
        case firstPress:
            PORTA = RED;
            _delay_ms(10);
            PORTA = GREEN;
            _delay_ms(10);
            releaseButton();
            break;
        case firstRelease:
            PORTA = GREEN; 
            pressButton();
            break;
        case secondPress:
            PORTA = RED; 
            releaseButton();
            break;
        case secondRelease:
            PORTA = OFF; 
            pressButton();
            break;
        case thirdPress:
            PORTA = GREEN;
            releaseButton();
            break;
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
