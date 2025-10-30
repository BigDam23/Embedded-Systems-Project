/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition des méthodes de la classe RS232
*/

#include "RS232.h"

void Communication::initialisationUART () 
{
    UBRR0H = 0;
    UBRR0L = 0xCF;
    UCSR0A |= (1<<MPCM0);
    UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
    UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
}

void Communication::transmissionUART (uint8_t donnee) 
{
    while ( !( UCSR0A & (1<<UDRE0)) );
    UDR0 = donnee;
}