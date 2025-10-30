/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition de la classe, qui permet la motricité du robot
*/
#pragma once
#include <avr/io.h>

enum class Direction { FRONT, BACK, LEFT, RIGHT };

class Motors
{
public:
    Motors(uint8_t leftSpeed, uint8_t rightSpeed);
    void move(Direction direction);
    void setLeftSpeed(uint8_t leftSpeed);
    void setRightSpeed(uint8_t rightSpeed);
    void setSameSpeed(uint8_t sameSpeed);
    void setNoSpeed();
    void turnLeft90();
    void turnRight90();
    void boostMotors(Direction direction);
    Direction getDirection();

private:
    void setPwm();
    uint8_t leftSpeed_;
    uint8_t rightSpeed_;
    Direction direction_;
};