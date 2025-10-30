/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition de la classe Debug, qui permet d'afficher des valeurs
grâce à la communication RS232 dans le but de facilité le débugage
*/

#include <avr/io.h>

class Debug{
public:
    // Défiitions des méthodes statiques de classes
    // Les méthodes sont statiques, car il n'y a pas
    // d'avantages a créer un objet pour l'appel de ces fonctions
    static void print(const char* text, uint8_t data);
    static void print(uint8_t data);
    static void print(const char* text);
    
private:
    // Il n'y a pas d'attributs de classe
};