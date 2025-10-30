/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition des méthodes de movement
Description:
executeHouse permet de definir ce que le robot fait durant une etape
nextEtape permet de definir ce que fait le robot entre des etapes ainsi que le changement d'etat
*/
#define F_CPU 8000000UL
#include <util/delay.h>
#include "movement.h"
#include "debug.h"

const uint8_t turnLeftValue1 = 0b11000;
const uint8_t turnLeftValue2 = 0b10000;
const uint8_t turnLeftValue3 = 0b01000;
const uint8_t turnLeftValue4 = 0b01100;
const uint8_t turnLeftValue5 = 0b11100;
const uint8_t turnRightValue1 = 0b00011;
const uint8_t turnRightValue2 = 0b00001;
const uint8_t turnRightValue3 = 0b00010;
const uint8_t turnRightValue4 = 0b00110;
const uint8_t turnRightValue5 = 0b00111;
const uint8_t readMiddle = 0b00100;
const uint8_t readNotLeft = 0b11110;
const uint8_t readNotRight = 0b01111;

const uint8_t delayMs200 = 200;
const uint8_t delayMs250 = 250;
const uint16_t delayMs300 = 300;
const uint16_t delayMs500 = 500;
const uint16_t delayMs700 = 700;
const uint16_t delayCentreRobot = 2100;
const uint16_t delayTurnToPost = 600;

const uint8_t normalSpeed = 70;
const uint8_t fastSpeed = 90;
const uint8_t slowSpeed = 60;
const uint8_t turnSpeed = 75;
const uint8_t veryFastSpeed = 100;

const uint8_t nLoopCenter = 220;
const uint8_t nLoopCenterShort = 60;
const uint8_t nLoopBalayScan = 50;

void moveWithLineDetector(uint8_t captorValue, Motors& motors){
    switch (captorValue) {
        
        case turnLeftValue2:
            if (motors.getDirection() != Direction::LEFT) motors.boostMotors(Direction::LEFT);
            motors.setSameSpeed(slowSpeed);
            break;

        case turnLeftValue1:
            if (motors.getDirection() != Direction::FRONT) motors.boostMotors(Direction::FRONT);
            motors.setRightSpeed(veryFastSpeed);
            motors.setLeftSpeed(slowSpeed);
            break;
        
        case turnLeftValue3: case turnLeftValue4: case turnLeftValue5:
            if (motors.getDirection() != Direction::FRONT) motors.boostMotors(Direction::FRONT);
            motors.setRightSpeed(fastSpeed);
            motors.setLeftSpeed(slowSpeed);
            break;
        
        case turnRightValue2:
            if (motors.getDirection() != Direction::RIGHT) motors.boostMotors(Direction::RIGHT);
            motors.setSameSpeed(slowSpeed);
            break;

        case turnRightValue1:
            if (motors.getDirection() != Direction::FRONT) motors.boostMotors(Direction::FRONT);
            motors.setRightSpeed(slowSpeed);
            motors.setLeftSpeed(veryFastSpeed);
            break;

        case turnRightValue3: case turnRightValue4: case turnRightValue5:
            if (motors.getDirection() != Direction::FRONT) motors.boostMotors(Direction::FRONT);
            motors.setLeftSpeed(fastSpeed);
            motors.setRightSpeed(slowSpeed);
            break;
        default:
            if (motors.getDirection() != Direction::FRONT) motors.boostMotors(Direction::FRONT);
            motors.setSameSpeed(normalSpeed);
            break;
    }
}

void alingCenter(LineDetector& lineDetector, Motors& motors){
    bool aligned = false;
    while(aligned == false){    
        uint8_t captorValue = lineDetector.readValue();
        switch (captorValue) {

            case turnLeftValue1: case turnLeftValue2: case turnLeftValue3: case turnLeftValue4:
                motors.boostMotors(Direction::LEFT);
                break;

            case turnRightValue1: case turnRightValue2: case turnRightValue3: case turnRightValue4:
                motors.boostMotors(Direction::RIGHT);
                break;

            default:
                aligned = true;
        }
        motors.setNoSpeed();
        _delay_ms(delayMs200);
    }
}

void turnUntilLine(Direction dir, Motors& motors, LineDetector& lineDetector) {
    Direction counterDir = dir == Direction::RIGHT ? Direction::LEFT : Direction::RIGHT;
    motors.setNoSpeed();
    _delay_ms(delayMs500);
    motors.boostMotors(dir);
    motors.setSameSpeed(turnSpeed);
    while (lineDetector.readValue() != noLine) {}
    while ((lineDetector.readValue() & readMiddle) == noLine) {}
    motors.boostMotors(counterDir);
    motors.move(Direction::FRONT);
}

void turnUntilLineNoVerif(Direction dir, Motors& motors, LineDetector& lineDetector) {
    Direction counterDir = dir == Direction::RIGHT ? Direction::LEFT : Direction::RIGHT;
    motors.setNoSpeed();
    _delay_ms(delayMs500);
    motors.boostMotors(dir);
    motors.setSameSpeed(turnSpeed);
    while ((lineDetector.readValue() & readMiddle) == noLine) {}
    motors.boostMotors(counterDir);
    motors.move(Direction::FRONT);
}

Direction turnUntilLineScan(Direction dir, Motors& motors, LineDetector& lineDetector, InfraredCaptor& infraredCaptor) {
    motors.setNoSpeed();
    _delay_ms(delayMs500);
    motors.boostMotors(dir);
    motors.setSameSpeed(turnSpeed);
    while (lineDetector.readValue() != noLine) {}
    _delay_ms(delayMs500);
    while ((lineDetector.readValue() & readMiddle) == noLine) {if(infraredCaptor.isPost()) {return Direction::LEFT;}}
    return Direction::RIGHT;
}



bool balayScan(Motors& motors, LineDetector& lineDetector, InfraredCaptor& infraredCaptor) {

    bool post = false;
    motors.setNoSpeed();
    _delay_ms(delayMs500);
    motors.boostMotors(Direction::RIGHT);
    motors.setSameSpeed(turnSpeed);
    for (uint8_t i = 0; i < nLoopBalayScan; i++){
        if (infraredCaptor.isPost()) post = true;
        _delay_ms(delayMs10);
    }
    turnUntilLineNoVerif(Direction::LEFT, motors, lineDetector);
    if (post) return true;

    motors.setNoSpeed();
    _delay_ms(delayMs500);
    motors.boostMotors(Direction::LEFT);
    motors.setSameSpeed(turnSpeed);
    for (uint8_t i = 0; i < nLoopBalayScan; i++){
        if (infraredCaptor.isPost()) post = true;
        _delay_ms(delayMs10);
    }
    turnUntilLineNoVerif(Direction::RIGHT, motors, lineDetector);
    if (post) return true;

    return false;
}

void centrerRobot(Motors& motors) {
    motors.boostMotors(Direction::FRONT);
    motors.setSameSpeed(normalSpeed);
    _delay_ms(delayCentreRobot);
}

void centrerRobotShort(Motors& motors){
    motors.boostMotors(Direction::FRONT);
    motors.setSameSpeed(normalSpeed);
    _delay_ms(delayMs700);
}

void centrerRobotWithLine(LineDetector& lineDetector, Motors& motors) {
    for (uint8_t i = 0; i < nLoopCenter; i++){
        moveWithLineDetector(lineDetector.readValue(), motors);
        _delay_ms(delayMs10);
    }
}

void centrerRobotWithLineShort(LineDetector& lineDetector, Motors& motors) {
    for (uint8_t i = 0; i < nLoopCenterShort; i++){
        moveWithLineDetector(lineDetector.readValue(), motors);
        _delay_ms(delayMs10);
    }
}

void confirmationPost(Led& led) {
    for (int i = 0; i < nbCyclesAlt; ++i) {
        led.setGreen();
        _delay_ms(delayAlt);
        led.setOff();
        _delay_ms(delayAlt);
    }
}

Direction scanPostsGrille(Motors& motors, InfraredCaptor& infraredCaptor, LineDetector& lineDetector){
    Led led('A', pinLed1, pinLed2);
    Direction dir;
    motors.setNoSpeed();
    _delay_ms(delayMs200);
    bool post = false;
    alingCenter(lineDetector, motors);
    if (infraredCaptor.isPost()) post = true;
    else if (balayScan(motors, lineDetector, infraredCaptor)) post = true;
    motors.setNoSpeed();

    if (post == false){
        confirmationPost(led);
        motors.setSameSpeed(normalSpeed);
        return Direction::FRONT;
    }

    dir = turnUntilLineScan(Direction::RIGHT, motors, lineDetector, infraredCaptor);
    motors.setNoSpeed();
    if (dir == Direction::RIGHT) {
        confirmationPost(led);
    }
    else {
        turnUntilLine(Direction::LEFT, motors, lineDetector);
    }
    return dir;
}

void advanceToMiddlePost(Motors& motors, LineDetector& lineDetector){
    uint8_t crossCounter = 0;

    motors.boostMotors(Direction::FRONT);
    motors.setSameSpeed(normalSpeed);
    while(crossCounter <= 1){
        if (lineDetector.readValue() == fullLine){
            crossCounter++;
            centrerRobotWithLine(lineDetector, motors);
        }
        moveWithLineDetector(lineDetector.readValue(), motors);
    }
}

void advanceToSidePosts(Motors& motors, Direction dir, LineDetector& lineDetector, bool isSecondCross){
    Led led('A', pinLed1, pinLed2);
    motors.setNoSpeed();
    Direction dir2;
    if(dir == Direction::RIGHT) dir2 = Direction::LEFT;
    else dir2 = Direction::RIGHT;
    uint8_t crossCounter = 0;

    if(dir == Direction::LEFT){
        turnUntilLine(dir, motors, lineDetector);
        motors.setNoSpeed();
        confirmationPost(led);
    }

    while(lineDetector.readValue() != noLine) {
        moveWithLineDetector(lineDetector.readValue(), motors);
    }

    centrerRobot(motors);
    turnUntilLine(dir2, motors, lineDetector);
    _delay_ms(300);

    while(crossCounter <= 1){
        if (((lineDetector.readValue() & turnLeftValue5) == turnLeftValue5) || 
            ((lineDetector.readValue() & turnRightValue5) == turnRightValue5)){
            crossCounter++;
            centrerRobotWithLine(lineDetector, motors);
        }
        moveWithLineDetector(lineDetector.readValue(), motors);
    }

    if(isSecondCross == false){ 
        turnUntilLine(dir2, motors, lineDetector);
        alingCenter(lineDetector, motors);

        while(lineDetector.readValue() != fullLine){
            moveWithLineDetector(lineDetector.readValue(), motors);
        }

        centrerRobotWithLine(lineDetector, motors);
        turnUntilLine(dir, motors, lineDetector);
    }
}

void advanceInGrille(Motors& motors, LineDetector& lineDetector, InfraredCaptor& infraredCaptor, bool isSecondCross){
    Direction direction = scanPostsGrille(motors, infraredCaptor, lineDetector);
    if (direction == Direction::FRONT) advanceToMiddlePost(motors, lineDetector);
    else advanceToSidePosts(motors, direction, lineDetector, isSecondCross);
}