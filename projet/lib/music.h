/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition de la classe, qui permet de jouer un son sur le buzzer
*/

#include <avr/io.h>
#define F_CPU 8000000UL

class Music {
public:
  Music();
  void playSound(uint8_t note);
  void stopSound();

private:
  double frequencesNotes[37] = {
    110, 116.54, 123.47, 130.81, 138.59, 146.84, 155.56, 164.81, 174.61, 184.99,
    195.99, 207.65, 220, 233.08, 246.94, 261.62, 277.18, 293.66, 311.12, 329.62,
    349.22, 369.99, 391.99, 415.30, 440, 466.16, 493.88, 523.25, 554.36, 587.32,
    622.25, 659.25, 698.45, 739.98, 783.99, 830.60, 880
  };
};