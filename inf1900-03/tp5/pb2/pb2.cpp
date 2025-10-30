#include <avr/io.h>
#include <util/delay.h>
void initialisationUART(void) {
    // 2400 bauds (UBRR = 207 ou 0xCF à 8 MHz)
    UBRR0H = 0;
    UBRR0L = 0xCF;

    // Activer la transmission et la réception (RXEN0 et TXEN0)
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Format des trames : 8 bits de données, 1 bit de stop, sans parité
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // UCSZ0[1:0] = 11 pour 8 bits
}

void transmissionUART(uint8_t donnee) {
    // Attendre que le tampon de transmission soit vide (TXC0)
    while (!(UCSR0A & (1 << UDRE0)));

    // Écrire l'octet à transmettre dans UDR0
    UDR0 = donnee;
}

void envoyerMessage() {
    char mots[21] = "Le robot en INF1900\n";
    uint8_t i, j;

    for (i = 0; i < 100; i++) {
        for (j = 0; j < 20; j++) {
            transmissionUART(mots[j]);
        }
    }
}
int main() {
    // Initialisation de l'UART
    initialisationUART();

    // Envoi du message en boucle
    envoyerMessage();

    return 0;
}

