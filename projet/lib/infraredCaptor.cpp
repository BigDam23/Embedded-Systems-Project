/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition des méthodes de la classe, qui permet la lecture du capteur infrarouge sur 8 bits
grace au convertisseur analogique-numerique
Broches materielles utilisees:
PA0
*/
#include "infraredCaptor.h"

InfraredCaptor::InfraredCaptor(){
    DDRA &= ~(1 << PA0);
}

uint8_t InfraredCaptor::readAnalogValue8Bits() {
    uint16_t captor16Bits = can_.lecture(0);
    uint8_t captor8Bits = can_.decalage(captor16Bits);
    return captor8Bits;
}

bool InfraredCaptor::isPost(){
    uint8_t value = readAnalogValue8Bits();
    if (value > minValue_){
        return true;
    }
    return false;
}