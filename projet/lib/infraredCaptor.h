/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition de la classe, qui permet la lecture du capteur infrarouge
*/
#include "can.h"
#include <avr/io.h>
#pragma once

class InfraredCaptor{
public:
    InfraredCaptor();
    uint8_t readAnalogValue8Bits();

    bool isPost();

private:
    can can_;
    uint8_t minValue_ = 36;
    //uint8_t maxValue_ = 110;
};
