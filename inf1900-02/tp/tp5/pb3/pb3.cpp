#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <memoire_24.h>
using namespace std;

const uint8_t STRING_LENGTH = 46;

void initialisationUART ( void ) {
    // 2400 bauds. Nous vous donnons la valeur des deux
    // premiers registres pour vous éviter des complications.
    UBRR0H = 0;
    UBRR0L = 0xCF;
    // permettre la réception et la transmission par le UART0
    UCSR0A |= (1<<MPCM0);
    UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
    // Format des trames: 8 bits, 1 stop bits, sans parité
    UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
}

// Du USART vers le PC
void transmissionUART (uint8_t donnee) 
{
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0 = donnee;
}

int main()
{
    initialisationUART();

    Memoire24CXXX memoire;

    uint8_t message[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
    memoire.ecriture(0, message, STRING_LENGTH);

    _delay_ms(5);
    
    uint8_t messageLecture[STRING_LENGTH];
    memoire.lecture(0, messageLecture, STRING_LENGTH);

    for (int i = 0; i < STRING_LENGTH; i++)
    {
        transmissionUART(messageLecture[i]);
    }
}