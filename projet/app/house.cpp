/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition des méthodes de House
Description:
executeHouse permet de definir ce que le robot fait durant une etape
nextEtape permet de definir ce que fait le robot entre des etapes ainsi que le changement d'etat

+--------------+-------------+--------------+
| État présent | captorValue | État suivant |
+--------------+-------------+--------------+
| BEGINING     | 0b--111     | FIRST_TURN   |
| FIRST_TURN   | 0b--111     | INTERSECT    |
| INTERSECT    |    -        | SECOND_TURN  |
| SECOND_TURN  | 0b11111     | LAST_TURN    |
| LAST_TURN    | 0b00000     | END          |
| END          |    -        | -            |
+--------------+-------------+--------------+

*/
#include "house.h"

const uint8_t mask = 0b00111;
const uint16_t delay3000Ms = 3000;

void executeHouse(LineDetector& lineDetector, Motors& motors, InfraredCaptor& infraredCaptor) {

    while ((lineDetector.readValue() & mask) == mask) {}

    StageHouse stage = StageHouse::BEGINING;

    Led led('A', pinLed1, pinLed2);

    while (stage != StageHouse::END) {
        uint8_t captorValue = lineDetector.readValue();
        switch (stage) {

        case StageHouse::BEGINING:
            if ((captorValue & mask) == mask) nextStage(stage, lineDetector, motors);
            else moveWithLineDetector(captorValue, motors);
            break;


        case StageHouse::FIRST_TURN:
            if ((captorValue & mask) == mask) nextStage(stage, lineDetector, motors);
            else moveWithLineDetector(captorValue, motors);
            break;


        case StageHouse::INTERSECT:
            advanceInIntersect(lineDetector, motors, infraredCaptor, led);
            nextStage(stage, lineDetector, motors);
            break;


        case StageHouse::SECOND_TURN:
            if (captorValue == fullLine) nextStage(stage, lineDetector, motors);
            else moveWithLineDetector(captorValue, motors);
            break;

        case StageHouse::LAST_TURN:
            if (captorValue == noLine) nextStage(stage, lineDetector, motors);
            else moveWithLineDetector(captorValue, motors);
            break;

        case StageHouse::END:
            break;
        }
    }
}

void nextStage(StageHouse& stage, LineDetector& lineDetector, Motors& motors) {
    switch (stage) {

    case StageHouse::BEGINING:
        centrerRobotWithLine(lineDetector, motors);
        turnUntilLine(Direction::RIGHT, motors, lineDetector);
        stage = StageHouse::FIRST_TURN;
        break;

    case StageHouse::FIRST_TURN:
        centrerRobot(motors);
        turnUntilLine(Direction::RIGHT, motors, lineDetector);
        stage = StageHouse::INTERSECT;
        break;

    case StageHouse::INTERSECT:
        centrerRobotShort(motors);
        turnUntilLine(Direction::RIGHT, motors, lineDetector);
        stage = StageHouse::SECOND_TURN;
        break;

    case StageHouse::SECOND_TURN:
        centrerRobot(motors);
        turnUntilLine(Direction::RIGHT, motors, lineDetector);
        stage = StageHouse::LAST_TURN;
        break;

    case StageHouse::LAST_TURN:
        stage = StageHouse::END;
        break;

    case StageHouse::END:
        break;

    }
}

void advanceInIntersect(LineDetector& lineDetector, Motors& motors, InfraredCaptor& infraredCaptor, Led& led) {
    motors.setNoSpeed();
    bool post = false;
    if (infraredCaptor.isPost()) post = true;
    else if (balayScan(motors, lineDetector, infraredCaptor)) post = true;
    motors.setNoSpeed();

    if (post) {
        for (int i = 0; i < nbCyclesAlt; ++i) {
            led.setRed();
            _delay_ms(delayAlt);
            led.setOff();
            _delay_ms(delayAlt);
        }
        turnUntilLine(Direction::RIGHT, motors, lineDetector);
    }

    else {
        for (int i = 0; i < nbCyclesAlt; ++i) {
            led.setGreen();
            _delay_ms(delayAlt);
            led.setOff();
            _delay_ms(delayAlt);
        }
    }

    motors.boostMotors(Direction::FRONT);
    while (lineDetector.readValue() != noLine) {
        moveWithLineDetector(lineDetector.readValue(), motors);
    }
}