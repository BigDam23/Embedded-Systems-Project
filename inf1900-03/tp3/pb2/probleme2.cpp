#define F_CPU 8000000UL // Fréquence du microcontrôleur (8 MHz)
#include <avr/io.h>
#include <util/delay.h>

#define FREQ1 60    // 60 Hz
#define FREQ2 400   // 400 Hz

void delay_ms_variable(uint16_t ms) {
    while (ms--) {
        _delay_ms(1); // Petite boucle pour contourner la restriction de _delay_ms()
    }
}

void PWM_manual(uint16_t freq) {
    uint16_t duty_cycles[] = {0, 25, 50, 75, 100};
    uint16_t period = 1000 / freq; // Période en ms
    uint16_t cycles = (2*1000) / period; //nombre de cycles en 2 secondes

    for (uint8_t i = 0; i < 5; i++) {
        uint16_t high_time = (period * duty_cycles[i]) / 100;
        uint16_t low_time = period - high_time;
        for (uint16_t j = 0; j < cycles; j++){
            PORTB |= (1 << PB0);  // HIGH sur PB0 et PB1
            delay_ms_variable(high_time);
            PORTB &= ~(1 << PB0); // LOW
            delay_ms_variable(low_time);
        }
    }
}

int main() {
    DDRB |= (1 << PB0) | (1 << PB1); // Configurer PB0 et PB1 en sortie

    
    while (1) {
        PWM_manual(FREQ1); // 60 Hz pendant 10 secondes
        PWM_manual(FREQ2); // 400 Hz pendant 10 secondes
    }
    
}
