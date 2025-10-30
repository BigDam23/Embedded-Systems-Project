/*
Table d'état
+--------------+-----------------+--------------+---------+
| État présent | isButtonPressed | État suivant | Lumière |
+--------------+-----------------+--------------+---------+
| RED          | 1               | OFF          | rouge   |
| RED          | 0               | AMBER        | rouge   |
| AMBRE        | 1               | GREEN        | ambre   |
| AMBRE        | 0               | X            | X       |
| GREEN        | 1               | ROUGE        | vert    |
| GREEN        | 0               | ROUGE        | vert    |
| OFF          | 1               | X            | X       |
| OFF          | 0               | GREEN        | off     |
+--------------+-----------------+--------------+---------+
*L'état change seulement lorsque isButtonPressed change de valeur
**(Si isButtonPressed est à 1, l'état change lorsque isButtonPressed devient 0)

PA0 (+)
PA1 (-)
*/

#define F_CPU 8000000UL
#include <avr/io.h> 
#include <util/delay.h> 
using namespace std;

enum class Color {RED, AMBER, GREEN, OFF};

const uint8_t DELAY_DEBOUNCE = 10;
const uint8_t DELAY_AMBER = 5;
const uint8_t RED_LIGHT = 0x01;
const uint8_t GREEN_LIGHT = 0x02;
const uint8_t OFF_LIGHT = 0xFC;

bool isButtonPressed(){
    uint8_t scan1 = PIND & (1<<PD2);
    _delay_ms(DELAY_DEBOUNCE);
    uint8_t scan2 = PIND & (1<<PD2);
    return scan1 && scan2;
}

void activateRedLight()
{
    PORTA |= RED_LIGHT;
    PORTA &= ~GREEN_LIGHT;
}

void activateGreenLight()
{
    PORTA |= GREEN_LIGHT;
    PORTA &= ~RED_LIGHT;
}

int main(){

    DDRA |= (1<<PA0) | (1<<PA1);
    DDRD &= (0<<PD2); 

    Color color {Color::RED};

    while (true){
        switch (color){
            case Color::RED:
                if (!(isButtonPressed())) {
                    while (!(isButtonPressed())) {
                        activateRedLight();
                    }
                    color = {Color::AMBER};   
                    break;   
                }
    
                if (isButtonPressed()) {
                    while (isButtonPressed()) {
                        activateRedLight();
                    }               
                    color = {Color::OFF}; 
                    break;     
                } 

            case Color::AMBER:
                if (isButtonPressed()) {
                    while (isButtonPressed()) {
                        activateRedLight();
                        _delay_ms(DELAY_AMBER);
                        activateGreenLight();
                        _delay_ms(DELAY_AMBER);
                    }
                    color = {Color::GREEN};
                }
                break;

            case Color::GREEN:  
                if (!(isButtonPressed())) {
                    while (!(isButtonPressed())) {
                        activateGreenLight();
                    }
                    color = {Color::RED};
                    break;
                }

                if (isButtonPressed()) {
                    while (isButtonPressed()) {
                        activateGreenLight();
                    }
                    color = {Color::RED};
                    break;
                }


            case Color::OFF: 
                if (!(isButtonPressed())) {
                    while (!(isButtonPressed())) {
                        PORTA &= OFF_LIGHT;
                    }
                    color = {Color::GREEN};      
                }
                break;
        }
  
    }
    return 0; 
}