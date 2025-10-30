/*
+---------------+---------------+-----------------+--------------+---------+
| État présent  | buttonPressed | isButtonPressed | État suivant | Lumière |
+---------------+---------------+-----------------+--------------+---------+
| INIT          | 0             | x               | INIT         | OFF     |
| INIT          | 1             | 1               | INIT         | OFF     |
| INIT          | 1             | 0               | PUSH1        | OFF     |
| PUSH1         | 0             | x               | PUSH1        | OFF     |
| PUSH1         | 1             | 1               | PUSH1        | OFF     |
| PUSH1         | 1             | 0               | PUSH2        | OFF     |
| PUSH2         | 0             | x               | PUSH2        | OFF     |
| PUSH2         | 1             | 1               | PUSH2        | OFF     |
| PUSH2         | 1             | 0               | PUSH3        | OFF     |
| PUSH3 (2 sec) | x             | x               | INIT         | RED     |
+---------------+---------------+-----------------+--------------+---------+
La variable buttonPressed devient true quand le bouton est appuyé pour la première fois. Il permet de détecter si le bouton est rêlaché.

PA0 (+)
PA1 (-)
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
using namespace std;

const uint8_t DELAY_DEBOUNCE = 10;
const uint16_t DELAY_LIGHT = 2000;
const uint8_t GREEN_LIGHT = 0x02;

enum class states{INIT, PUSH1, PUSH2, PUSH3};

//Copié de l'exemple du chargé de laboratoire
bool isButtonPressed(){
    uint8_t scan1 = PIND & (1<<PD2);
    _delay_ms(DELAY_DEBOUNCE);
    uint8_t scan2 = PIND & (1<<PD2);
    return scan1 && scan2;
}

int main()
{
    DDRA |= (1<<PA0) | (1<<PA1);
    DDRD &= (0<<PD2);

    states state = states::INIT;
    bool buttonPressed = false;

    while(true)
    {
        switch(state)
        {
            case states::INIT:
                if (isButtonPressed())
                {
                    buttonPressed = true;
                }
                else if (buttonPressed)
                {
                    state = states::PUSH1;
                    buttonPressed = false;
                }
                break;
            case states::PUSH1:
                if (isButtonPressed())
                {
                    buttonPressed = true;
                }
                else if (buttonPressed)
                {
                    state = states::PUSH2;
                    buttonPressed = false;
                }
                break;
            case states::PUSH2:
                if (isButtonPressed())
                {
                    buttonPressed = true;
                }
                else if (buttonPressed)
                {
                    state = states::PUSH3;
                    buttonPressed = false;
                }
                break;
            case states::PUSH3:
                PORTA |= GREEN_LIGHT;
                _delay_ms(DELAY_LIGHT);
                PORTA &= ~GREEN_LIGHT;
                state = states::INIT;
                break;
        }
    }
    return 0;
}