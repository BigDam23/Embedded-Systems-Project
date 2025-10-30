/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Transmitter permettant de stocké dans la mémoire externe 
du robot un byte code envoyé de l'ordinateur par RS232
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

// importer les parties de la librairie
#include "../lib/controlLED.h"
#include "../lib/RS232.h"
#include "../lib/memoire_24.h"

int main()
{
    // variables
    DDRA=0x03;
    uint16_t dataAddress = 0x00;
    uint16_t codeSize;

    // objets
    Led led('A', 0, 1);
    Communication transmitter;
    transmitter.initialisationUART();
    Memoire24CXXX memory;

    //receive a byte
    uint8_t firstByte = transmitter.receive();
    codeSize = firstByte;

    //write a byte
    memory.ecriture(dataAddress, firstByte);
    dataAddress++;

    _delay_ms(5);

    //receive another byte
    uint8_t secondByte = transmitter.receive();
    //write another byte
    memory.ecriture(dataAddress, secondByte);
    dataAddress++;

    _delay_ms(5);

    // calcul du nb total d'octets
    codeSize = ((codeSize << 8) | secondByte);


    while (dataAddress < codeSize)
    {
        //recoit
        uint8_t dataReceived = transmitter.receive();
        //ecrit
        memory.ecriture(dataAddress, dataReceived);

        _delay_ms(5);
        // prochaine adresse
        dataAddress++;
    }
    //allume la led en vert pour confirmer la fin de la transmission
    led.setGreen();
    _delay_ms(1000);
    led.setOff();
}