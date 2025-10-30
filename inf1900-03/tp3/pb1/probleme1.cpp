#define F_CPU 8000000UL   // Fréquence du microcontrôleur (8 MHz)
#include <avr/io.h>
#include <util/delay.h>

#define PWM_FREQ 1000     // Fréquence du PWM (1 kHz)
#define PERIOD_US (1000000UL / PWM_FREQ) // Période en microsecondes (1 ms)

void delay_us_variable(uint16_t us) {
    while (us--) {
        _delay_us(1);
    }
}

void attenuer_DEL(uint8_t pin1, uint8_t pin2) {
    for (uint16_t t = 0; t < 3000; t += 30) { // 3 secondes, 30 ms par step (100 steps)
        uint8_t duty_cycle = 100 - (t / 30); // Diminue progressivement de 100 à 0
        
        uint16_t high_time = (PERIOD_US * duty_cycle) / 100;
        uint16_t low_time = PERIOD_US - high_time;

        for (uint8_t i = 0; i < 30; i++) { // 30 itérations = 30 ms
            PORTB |= ((1 << pin1) |(0 << pin2));   // ON (HIGH)
            delay_us_variable(high_time);
            PORTB &= ~((1 << pin1) |(1 << pin2));  // OFF (LOW)
            delay_us_variable(low_time);
        }
    }
}

int main() {
    DDRB |= (1 << PB0) | (1 << PB1); // Configurer PB0 et PB1 en sortie
    while (1) {
        attenuer_DEL(PB1, PB0); // Atténuation de la DEL rouge sur 3 secondes
        _delay_ms(500);    // Pause entre les deux
        attenuer_DEL(PB0, PB1); // Atténuation de la DEL verte sur 3 secondes
        _delay_ms(500);    // Pause avant de recommencer
    }
}
