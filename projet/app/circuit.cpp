/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition des méthodes de Circuit
Description:
executeCircuit est une machine à état qui défini ce que le robot doit faire selon la lecture 
du détecteur de ligne ainsi que l'étape du circuit où le robot se trouve.
nextState permet de changer l'état du robot ainsi que de faire des ajustements au robot avant
de passer à l'étape suivante.

La tablé d'état suivante illustre les changements d'état du robot selon la valeur du capteur 
de ligne. Il est à noter que captorValue est évalué dans la fonction executeCircuit alors que
le changement d'état de fait dans nextState.
Tableau d'etat: 
+--------------+-------------+--------------+
| État présent | captorValue | État suivant |
+--------------+-------------+--------------+
| TOP_LEFT     | 0b00000     | FORK_1       |
| FORK_1       | 0b00000     | FORK_2       |
| FORK_2       | 0b00000     | TOP_RIGHT    |
| TOP_RIGHT    | 0b00000     | HOUSE        |
| HOUSE        |    -        | BOTTOM_RIGHT |
| BOTTOM_RIGHT | 0b11111     | CROSS_1      |
| CROSS_1      |    -        | CROSS_2      |
| CROSS_2      |    -        | LEFT_SIDE    |
| LEFT_SIDE    | 0b00000     | TOP_LEFT     |
+--------------+-------------+--------------+
*/

#include "circuit.h"
#include <util/delay.h>

const uint8_t normalSpeed = 110;

const uint8_t normalRight = 0b00111;

const uint8_t loopDelay = 10;
const uint8_t nLoopTimes = 200;
const uint16_t delay1000Ms = 1000;
const uint8_t musicFrequence = 50;

void executeCircuit(Stages stage, LineDetector& lineDetector, Motors& motors, StateMachine& stateMachine, InfraredCaptor& infraredCaptor){
    Stages endStage = stage;
    nextStage(stage, lineDetector, motors, stateMachine.getDirection1(), infraredCaptor);

    bool isEndFork2 = false;

    while (stage != endStage){
        uint8_t captorValue = lineDetector.readValue();
        switch (stage){

            case Stages::TOP_LEFT:
                if (captorValue == noLine) nextStage(stage, lineDetector, motors, stateMachine.getDirection1(), infraredCaptor);
                else moveWithLineDetector(captorValue, motors);
                break;   

            case Stages::FORK_1:
                if (captorValue == noLine) nextStage(stage, lineDetector, motors, stateMachine.getDirection2(), infraredCaptor);
                else moveWithLineDetector(captorValue, motors);
                break; 

            case Stages::FORK_2:
                Direction dir;
                if ((captorValue == noLine) && isEndFork2) nextStage(stage, lineDetector, motors, Direction::FRONT, infraredCaptor);
                else if ((captorValue == noLine) && !isEndFork2) {
                    if (stateMachine.getDirection2() == Direction::RIGHT) {dir = Direction::LEFT;}
                    else {dir = Direction::RIGHT;}
                    centrerRobotShort(motors);
                    turnUntilLine(dir, motors, lineDetector);
                    isEndFork2 = true;
                }
                else moveWithLineDetector(captorValue, motors);
                break; 

            case Stages::TOP_RIGHT:
                if ((captorValue & normalRight) == normalRight) nextStage(stage, lineDetector, motors, Direction::FRONT, infraredCaptor);
                else moveWithLineDetector(captorValue, motors);               
                break;

            case Stages::HOUSE:
                executeHouse(lineDetector, motors, infraredCaptor);
                nextStage(stage, lineDetector, motors, Direction::FRONT, infraredCaptor);
                break;

            case Stages::BOTTOM_RIGHT:
                if (captorValue == fullLine) nextStage(stage, lineDetector, motors, Direction::FRONT, infraredCaptor);
                else moveWithLineDetector(captorValue, motors);
                break; 

            case Stages::CROSS_1:
                advanceInGrille(motors, lineDetector, infraredCaptor, false);
                nextStage(stage, lineDetector, motors, Direction::FRONT, infraredCaptor);
                break;
            
            case Stages::CROSS_2:
                advanceInGrille(motors, lineDetector, infraredCaptor, true);
                nextStage(stage, lineDetector, motors, Direction::FRONT, infraredCaptor);
                break;

            case Stages::LEFT_SIDE:
                if (captorValue == noLine) nextStage(stage, lineDetector, motors, Direction::FRONT, infraredCaptor);
                else moveWithLineDetector(captorValue, motors);
                break; 
            }
    }
}

void nextStage(Stages& stage, LineDetector& lineDetector, Motors& motors, Direction direction, InfraredCaptor& infraredCaptor){
    motors.setSameSpeed(normalSpeed);
    Music music;
    switch (stage){
        case Stages::TOP_LEFT:
            centrerRobotShort(motors);
            motors.setNoSpeed();
            music.playSound(musicFrequence);
            _delay_ms(delay1000Ms);
            music.stopSound();
            turnUntilLine(direction, motors, lineDetector);
            stage = Stages::FORK_1;
            break;
            
        case Stages::FORK_1:
            centrerRobotShort(motors);
            motors.setNoSpeed();
            music.playSound(musicFrequence);
            _delay_ms(delay1000Ms);
            music.stopSound();
            turnUntilLine(direction, motors, lineDetector);
            stage = Stages::FORK_2;           
            break;

        case Stages::FORK_2:
            centrerRobot(motors);
            turnUntilLine(Direction::RIGHT, motors, lineDetector);
            stage = Stages::TOP_RIGHT;
            break;

        case Stages::TOP_RIGHT:
            centrerRobotWithLine(lineDetector, motors);
            stage = Stages::HOUSE;
            break;

        case Stages::HOUSE:
            centrerRobot(motors);
            turnUntilLine(Direction::RIGHT, motors, lineDetector);
            stage = Stages::BOTTOM_RIGHT;           
            break;

        case Stages::BOTTOM_RIGHT:
            while (lineDetector.readValue() == fullLine) {}
            alingCenter(lineDetector, motors);
            for (uint8_t i = 0; i < nLoopTimes; i++){
                moveWithLineDetector(lineDetector.readValue(), motors);
                _delay_ms(loopDelay);
            }
            alingCenter(lineDetector, motors);
            stage = Stages::CROSS_1; 
            break;

        case Stages::CROSS_1:
            stage = Stages::CROSS_2;
            break;
        
        case Stages::CROSS_2:
            turnUntilLine(Direction::RIGHT, motors, lineDetector);
            stage = Stages::LEFT_SIDE;
            break;

        case Stages::LEFT_SIDE:
            centrerRobot(motors);
            turnUntilLine(Direction::RIGHT, motors, lineDetector);
            stage = Stages::TOP_LEFT;
            break;
    }
}
