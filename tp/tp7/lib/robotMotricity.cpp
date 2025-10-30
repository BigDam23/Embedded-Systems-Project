/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition des méthodes de la classe, qui permet la motricité du robot
*/

#include <avr/io.h>
#include "robotMotricity.h"

Motors::Motors(uint8_t leftSpeed, uint8_t rightSpeed)
    : leftSpeed_(leftSpeed), rightSpeed_(rightSpeed)
{
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB4) | (1 << PB6);
    setPwm();
}

void Motors::setPwm(){
    OCR0A = leftSpeed_;
    OCR0B = rightSpeed_;
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00);
    TCCR0B = (1 << CS01);
}

void Motors::setLeftSpeed(uint8_t leftSpeed){
    leftSpeed_ = leftSpeed;
    OCR0B = leftSpeed_;
}

void Motors::setRightSpeed(uint8_t rightSpeed){
    rightSpeed_ = rightSpeed;
    OCR0A = rightSpeed_;
}

void Motors::setSameSpeed(uint8_t sameSpeed){
    setLeftSpeed(sameSpeed);
    setRightSpeed(sameSpeed);
}

void Motors::setNoSpeed(){
    setSameSpeed(0);
}

void Motors::move(Direction direction) {
    //PB5 = direction roue droite (0 = avance et 1 = recule)
    //PB6 = direction roue gauche (0 = avance et 1 = recule)
    switch (direction) {
        case Direction::FRONT:
            PORTB &= ~((1 << PB5) | (1 << PB6));
            break;
        case Direction::BACK:
            PORTB |= (1 << PB5) | (1 << PB6);
            break;
        case Direction::LEFT:
            PORTB |= (1 << PB6);
            PORTB &= ~(1 << PB5);
            break;
        case Direction::RIGHT:
            PORTB |= (1 << PB5);
            PORTB &= ~(1 << PB6);
            break;
    }
    setPwm();
}
