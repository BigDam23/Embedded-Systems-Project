#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
using namespace std;

const uint8_t FORWARD = 0x04;

const uint16_t TIME = 2000;
const double PERIOD1 = 16.5;
const double PERIOD2 = 2.5;
const uint8_t STARTING_PERCENTAGE = 0;
const uint8_t STEP_PERCENTAGE = 25;
const uint8_t DELAY = 10;

void PWM(double period)
{
    uint8_t  percentage = STARTING_PERCENTAGE;
    uint16_t nIter = TIME / period;
    for(uint8_t h = 0; h < 5; h++)
    {
        uint16_t a = period * percentage;
        uint16_t timeOff = period * 100 - a;
        for (uint16_t i = 0; i < nIter; i++)
        {
            PORTA |= FORWARD;
            for (uint16_t j = 0; j < a; j++)
            {
                _delay_us(DELAY);
            }
            PORTA &= ~FORWARD;
            for (uint16_t l = 0; l < timeOff; l++)
            {
                _delay_us(DELAY);
            }
        }
        percentage += STEP_PERCENTAGE;
    }
}

int main()
{
    DDRA |= (1 << PA2) | (1 << PA3);

    while(true)
    {
        PWM(PERIOD1);
        PWM(PERIOD2);
    }

    return 0;
}
