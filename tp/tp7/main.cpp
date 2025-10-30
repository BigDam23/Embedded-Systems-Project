
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "../lib/controlLED.h"
#include "../lib/configInterruption.h"
#include "../lib/robotMotricity.h"
#include "../lib/RS232.h"
#include "../lib/debug.h"
#include "../lib/music.h"
#include <avr/interrupt.h>

#ifdef DEBUG
    // Initialise la communication RS232 pour permettre
    // à la fonction print d'afficher de valeurs
    #define DEBUG_INIT Communication::initialisationUART()

    // Fonction print permet d'afficher avec RS232
    #define DEBUG_PRINT(text, data) Debug::print(text, data)                           
#else
    #define DEBUG_INIT do {} while (0) //code mort
    #define DEBUG_PRINT(text, data) do {} while (0) //code mort
#endif

volatile bool gButtonPressed = false;
volatile bool gTimerCheck = false;

ISR(INT0_vect)
{
    gButtonPressed = true;
}

ISR(TIMER1_COMPA_vect)
{
    gTimerCheck = true;
}

int main()
{
    /*___SECTION TEST MUSIQUE___
    Music music;

    uint8_t noteDuration = 200; // 200 ms par note
    // Séquence de notes de votre choix, par exemple, C, D, E, F, G, A, B
    for (int note = 45; note <= 81; ++note)
    {
        music.playSound(note);
        _delay_ms(noteDuration);
        music.stopSound();
        _delay_ms(10);
    }*/
    
    /*___SECTION TEST DEL___
    Led led('A', 0, 1);
    led.setGreen();
    _delay_ms(1000);
    led.setRed();
    _delay_ms(1000);
    led.setOff();*/

    /*___SECTION TEST INTERRUPTIONS___
    Interruption::setINT0();
    Interruption::delay(2000);
    while(!gButtonPressed)
    {
        if(gTimerCheck)
        {
            led.setRed();
            gTimerCheck = false;
            _delay_ms(200);
            led.setOff();
        }
    }*/
  
    /*___SECTION TEST ROBOT MOTRICITY___*/
    // Tester si le robot avance bien dans les
    // 4 directions pendant 5 secondes à 50% de vitesse
    Motors motors(128,128);
    motors.move(Direction::FRONT);
    _delay_ms(5000);
        
    motors.move(Direction::BACK);
    _delay_ms(5000);

    motors.move(Direction::FRONT);
    _delay_ms(5000);

    motors.move(Direction::LEFT);
    _delay_ms(5000);

    motors.move(Direction::RIGHT);
    _delay_ms(5000);

    motors.setNoSpeed();

    /*___SECTION TEST DEBUG___*/
    // La fonction print définie par le debug
    // utilise la communication RS232, donc pas
    // besoin de test suplémentaire pour RS232
    DEBUG_INIT;
    DEBUG_PRINT("La valeur est : ", 200);
    
    return 0;
}