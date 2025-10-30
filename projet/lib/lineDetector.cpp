/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition des méthodes de la classe, qui permet la lecture du lineDetector sur 5 bits
Broches materielles utilisees:
PC0 pour le 1er bit, PC1 pour le 2eme bit, PC2 pour le 3eme bit, PC3 pour le 4eme bit et PC4 pour le 5eme bit.
*/
#include "lineDetector.h"

const uint8_t MASK_CAPTEUR = 0x1f;

LineDetector::LineDetector(){
    DDRC &= ~((1 << PC0)|(1 << PC1)|(1 << PC2)|(1 << PC3)|(1 << PC4));
}

uint8_t LineDetector::readValue(){
    return PINC & MASK_CAPTEUR;
}