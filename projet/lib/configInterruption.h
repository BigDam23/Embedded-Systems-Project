/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition de la classe Interruption, qui permet de configurer les interruptions et le Timer 1 en mode CTC
*/
#include <avr/io.h>

class Interruption
{
public:
    static void setINT0();
    static void startTimer1(uint16_t time, bool prescaler, uint8_t mode);
    static void delay(uint16_t time);
    static void stopTimer1();
};