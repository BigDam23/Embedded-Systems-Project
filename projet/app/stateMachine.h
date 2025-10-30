/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition de la classe, qui decrit la problematique du debut du circuit en machine a etat.
*/
#pragma once

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "controlLED.h"
#include "robotMotricity.h"
#include "constants.h"

enum State {INIT, FIRST_PRESS_LEFT, FIRST_PRESS_RIGHT, FIRST_RELEASE, SECOND_PRESS_LEFT, SECOND_PRESS_RIGHT, SECOND_RELEASE};

class StateMachine{
public:
    StateMachine();

    void run();

    Direction getDirection1() const;
    Direction getDirection2() const;

private:
    bool isButtonPressed(uint8_t buttonChoice);

    State state;
    bool endMachine;
    bool direction1;
    bool direction2;
    Led led;
};