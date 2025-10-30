/* pour probleme 2
p = 1/f
a = p * pourcentage
timeOff = periode - a
nIter = nSeconde/periode
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
using namespace std;

const uint8_t RED_LIGHT = 0x01;
const uint8_t GREEN_LIGHT = 0x02;

const uint16_t TIME = 3000;
const uint8_t PERIOD = 1;
const double STARTING_PERCENTAGE = 100;
const uint8_t DELAY = 10;

void PWM(uint8_t light)
{
    uint16_t nIter = TIME / PERIOD;
    double percentage = STARTING_PERCENTAGE;
    double ratio = STARTING_PERCENTAGE/nIter;

    for (uint16_t i = 0; i < nIter; i++)
    {
        uint16_t a = PERIOD * percentage;
        uint16_t timeOff = PERIOD * STARTING_PERCENTAGE  - a;
        percentage -= ratio;

        PORTA |= light;
        for (uint16_t j = 0; j < a; j++)
        {
            _delay_us(DELAY);
        }
        PORTA &= ~light;
        for (uint16_t l = 0; l < timeOff; l++)
        {
            _delay_us(DELAY);
        }
    }
}

int main()
{
    DDRA |= (1 << PA0) | (1 << PA1);

    while (true)
    {
        PWM(RED_LIGHT);
        PWM(GREEN_LIGHT);
    }

    return 0;
}