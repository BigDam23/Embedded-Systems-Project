/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

TP7

Définition des méthodes de la classe Del
*/

#include <avr/io.h>
#include "controlLED.h"

const uint8_t DDR = 1;
const uint8_t PORT = 2;

// Constructeur de l'objet Del
// Prend en paramètre la lettre correspondant au
// bon port sur la carte mère ainsi que le numéro
// des deux pins à utiliser
Led::Led(char pin, uint8_t pinN1, uint8_t pinN2){
    // Définir les valeurs des 2 attributs pinN1_ et pinN2_
    pinN1_ = pinN1;
    pinN2_ = pinN2;

    // Définir la valeur de l'attribut pin_
    switch (pin){
        case 'A':
            pin_ = 0x00;
            break;
        case 'B':
            pin_ = 0x03;
            break;
        case 'C':
            pin_ = 0x06;
            break;
        case 'D':
            pin_ = 0x09;
            break;
    }

    // La fonction _SFR_IO8 est la fonction qui se cache 
    // derrière les define DDR, PIN et PORT
    // La documentation ce trouve au lien ci-dessous:
    // https://github.com/avrdudes/avr-libc/blob/main/include/avr/iom324pa.h
    // En utilisant cette fonction directement, on peut associer une valeur
    // numérique à chaque port nous permettant ainsi d'éviter des 
    // switch-case à répétition
    _SFR_IO8(pin_ + DDR) |= (1<<pinN1_) | (1<<pinN2_); //Équivalent à DDRX |= ...
}

// Fonction pour faire tourner la DEL au rouge
void Led::red(){
    _SFR_IO8(pin_ + PORT) |= (1<<pinN1_); //Équivalent à PORTX |= ...
    _SFR_IO8(pin_ + PORT) &= ~(1<<pinN2_); //Équivalent à PORTX &= ...
}

// Fonction pour faire tourner la DEL au vert
void Led::green(){
    _SFR_IO8(pin_ + PORT) |= (1<<pinN2_); //Équivalent à PORTX |= ...
    _SFR_IO8(pin_ + PORT) &= ~(1<<pinN1_); //Équivalent à PORTX &= ...
}

// Fonction pour éteindre la DEL
void Led::off(){
    _SFR_IO8(pin_ + PORT) &= ~((1<<pinN1_) | (1<<pinN2_)); //Équivalent à PORTX &= ...
}