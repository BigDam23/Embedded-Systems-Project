/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition de la classe Interruption, qui permet de configurer les interruptions et le Timer 1 en mode CTC
*/

class Interruption
{
public:
    static void INT0Setter();
    static void timer1Start(uint16_t time, bool prescaler, uint8_t mode);
    static void delay(uint16_t time);
    static void timer1Stop();
};