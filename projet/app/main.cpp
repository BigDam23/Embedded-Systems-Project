/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Main du projet
Broches materielles utilisees:
Led : utilise les ports PA2 et PA3, le positif est (broche +) et le negatif est (broche -)
Moteur gauche : PB3 et PB5 pour le PWM et la direction respectivement
Moteur droite : PB4 et PB6 pour le PWM et la direction respectivement
LineDetector : PC0, PC1, PC2, PC3 et PC4
BouttonPoussoir sur breadboard : PC7
Capteur infrarouge : PA0
Emetteur de son : PD4 et PD5

Description du programme :
Declare les objets utilises au travers du circuit
Appelle le circuit selon l'etape a laquelle le robot commence
S'occupe de la LED finale
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"
#include "RS232.h"
#include "circuit.h"

const uint8_t delayLed = 250;
const uint8_t nbCyclesLed = 4;
const uint16_t delay2000Ms = 2000;

int main(){
    StateMachine stateMachine;
    Motors motors(0, 0);
    LineDetector lineDetector;
    InfraredCaptor infraredCaptor;
    Communication::initialisationUART();
    Led led('A', pinLed1, pinLed2);
    led.setOff();

    stateMachine.run();
    _delay_ms(delay2000Ms);

    motors.boostMotors(Direction::FRONT);
    motors.boostMotors(Direction::FRONT);
    motors.boostMotors(Direction::FRONT);

    while (true)
    {
        uint8_t captorValue = lineDetector.readValue();
        
        if (captorValue == fullLine) {
            executeCircuit(Stages::BOTTOM_RIGHT, lineDetector, motors, stateMachine, infraredCaptor);
            break;
        }
        else if (captorValue == noLine) {
            executeCircuit(Stages::TOP_LEFT, lineDetector, motors, stateMachine, infraredCaptor);
            break;
        }
        else {
            moveWithLineDetector(captorValue, motors);
        }
    }

    motors.setNoSpeed();
    alingCenter(lineDetector, motors);

    for (int i = 0; i < nbCyclesLed; ++i) {
        led.setRed();
        _delay_ms(delayLed);
        led.setGreen();
        _delay_ms(delayLed);
    }
    led.setOff();
    
}