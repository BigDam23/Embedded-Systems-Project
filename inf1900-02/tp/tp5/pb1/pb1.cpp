#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <memoire_24.h>
using namespace std;

const uint8_t STRING_LENGTH = 46;

const uint8_t RED_LIGHT = 0x01;
const uint8_t GREEN_LIGHT = 0x02;

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

int main()
{
    DDRA |= (1<<PA0) | (1<<PA1);

    Memoire24CXXX memoire;

    uint8_t message[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
    memoire.ecriture(0, message, STRING_LENGTH);

    _delay_ms(5);
    
    uint8_t messageLecture[STRING_LENGTH];
    memoire.lecture(0, messageLecture, STRING_LENGTH);

    if(*message == *messageLecture)
    {
        activateGreenLight();
    }
    else {activateRedLight();}
}