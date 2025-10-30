/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition de la classe, qui permet la communication par RS232
*/

#include <avr/io.h>

class Communication
{
public:
    static void initialisationUART();
    static void transmissionUART(uint8_t donnee);
    uint8_t receive();

};