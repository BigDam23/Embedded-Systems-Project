#include "music.h"

void Music::playSound(uint8_t note) {
    if (note < 45 || note > 81) return;
    double freq = frequencesNotes[note - 45];
    uint32_t calculatedTime = (F_CPU / (2. * 256 * freq)) - 1;
    TCNT1 = 0;
    OCR1A = calculatedTime;
    TCCR1A |= (1 << COM1A0);
    TCCR1B = (1 << WGM12) | (1 << CS12);
}

void Music::stopSound() {
    // Desactive la sortie 
    PORTD &= ~(1 << PD4);
    TCCR1A &= ~(1 << COM1A0);
}

Music::Music(){
    DDRD |= (1 << PD5) | (1 << PD4);
    PORTD &= ~(1 << PD5);
    PORTD |= (1 << PD4);
}