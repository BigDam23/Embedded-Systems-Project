/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition de la classe Del, qui permet le contrôle
de la DEL sur la care mère
*/

#include <avr/io.h>


class Led
{
public:
    // Définition des méthodes publiques:

    // Constructeur de la classe
    Led(char port, uint8_t pinN1, uint8_t pinN2);

    // Fonctions permettant le controles de la LED
    void setRed();
    void setGreen();
    void setOff();
    
private:
    // Attributs de la classe
    uint8_t pin_;
    uint8_t pinN1_, pinN2_;
};