/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition de la classe, qui permet la lecture du capteur de ligne
*/
#pragma once
#include "can.h"
#include <avr/io.h>

class LineDetector{
public:
    LineDetector();
    uint8_t readValue();
};
