/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Description:
Appuyer sur le bouton blanc indique de passer par la gauche, et appuyer sur le bouton Interrupt indique de passer par la droite. 
L’évaluateur aura donc à appuyer sur un bouton à deux reprises. Une première fois pour sélectionner le chemin à suivre suite à la rencontre du point B.
Puis, une deuxième fois pour déterminer le chemin à prendre après le point C.
L'évaluateur n'appuiera pas sur les deux boutons en même temps et
il s'assurera de relâcher complètement le bouton (après son premier choix) avant d’effectuer son deuxième choix.

Lorsqu'un des boutons est enfoncé, le robot ne doit pas seulement enregistrer la direction à prendre, mais il doit aussi allumer sa DEL en guise de confirmation.
La DEL doit rester allumée tant que le bouton n'est pas relâché. Rouge pour la gauche, vert pour la droite.

Une fois la deuxième sélection effectuée, le robot ne fait rien pendant 2 secondes. Après ce délai, il se met en marche.

DEL rouge : A3 = 0 et A2 = 1
DEL verte : A3 = 1 et A2 = 0
La branche positive de la DEL est associee a A0 et la branche negative de la DEL a A1.
Le bouton poussoir interrupt est associee au port D2.
Le bouton poussoir blanc est associe au port C7.

Tableau d'etat: 
+------------------+----+----+------------------+----+----+
| État présent     | C7 | D2 | État suivant     | A1 | A0 |
+------------------+----+----+------------------+----+----+
| INIT             | 0  | 1  | FirstPressRight  | 0  | 0  |
| INIT             | 1  | 0  | FirstPressLeft   | 0  | 0  |
| FirstPressLeft   | 0  | -  | FirstRelease     | 0  | 1  |
| FirstPressRight  | -  | 0  | FirstRelease     | 1  | 0  |
| FirstRelease     | 0  | 1  | SecondPressRight | 0  | 0  |
| FirstRelease     | 1  | 0  | SecondPressLeft  | 0  | 0  |
| SecondPressLeft  | 0  | -  | SecondRelease    | 0  | 1  |
| SecondPressRight | -  | 0  | SecondRelease    | 1  | 0  |
| SecondRelease    | -  | -  | Suite du code    | 0  | 0  |
+------------------+----+----+------------------+----+----+
*/

#include "stateMachine.h" 


StateMachine::StateMachine() : led('A', pinLed1, pinLed2){
    state = {State::INIT}; 
    endMachine = false;
    DDRD &= (0 << PD2);
    DDRC &= (0 << PC7);
    direction1 = false;
    direction2 = false;
}

Direction StateMachine::getDirection1() const {
    if(direction1) return Direction::RIGHT;
    else return Direction::LEFT;
}

Direction StateMachine::getDirection2() const {
    if(direction2) return Direction::RIGHT;
    else return Direction::LEFT;}


bool StateMachine::isButtonPressed(uint8_t buttonChoice){
    uint8_t scan1 = 0;
    uint8_t scan2 = 0;
    if (buttonChoice == 0){
        scan1 = PINC & (1<<PC7);
        _delay_ms(delayMs10);
        scan2 = PINC & (1<<PC7);
    }
    else {
        scan1 = PIND & (1<<PD2);
        _delay_ms(delayMs10);
        scan2 = PIND & (1<<PD2);
    }
    return scan1 && scan2;
}

void StateMachine::run(){
    while(endMachine == false){
        switch(state){
            case INIT:
                led.setOff();
                if (isButtonPressed(0)){
                    state = {State::FIRST_PRESS_LEFT};
                }
                if (isButtonPressed(1)){
                    state = {State::FIRST_PRESS_RIGHT};
                }
                break;
            
            case FIRST_PRESS_LEFT:
                direction1 = false;
                while(isButtonPressed(0)){
                    led.setRed();
                }
                state = {State::FIRST_RELEASE};
                break;
            
            case FIRST_PRESS_RIGHT:
                direction1 = true;
                while(isButtonPressed(1)){
                    led.setGreen();
                }
                state = {State::FIRST_RELEASE};
                break;
            
            case FIRST_RELEASE:
                led.setOff();
                if (isButtonPressed(0)){
                    state = {State::SECOND_PRESS_LEFT};
                }
                if (isButtonPressed(1)){
                    state = {State::SECOND_PRESS_RIGHT};
                }
                break;
            
            case SECOND_PRESS_LEFT:
                direction2 = false;
                while(isButtonPressed(0)){
                    led.setRed();
                }
                state = {State::SECOND_RELEASE};
                break;

            case SECOND_PRESS_RIGHT:
                direction2 = true;
                while(isButtonPressed(1)){
                    led.setGreen();
                }
                state = {State::SECOND_RELEASE};
                break;
            
            case SECOND_RELEASE:
                led.setOff();
                endMachine = true;
                break;
        }
    }
}
