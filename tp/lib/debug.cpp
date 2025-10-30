/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Définition des méthodes de la classe Debug
*/

#include "RS232.h"
#include "debug.h"
#include  <string.h>

const uint8_t NB_MAX_DIGIT = 3;

// Fonction pour print une chaine de characteres
void Debug::print(const char* text)
{
    for (uint8_t i = 0; i < strlen(text); i++)
        Communication::transmissionUART(text[i]);
}

// Fonction pour print un nombre decimal
void Debug::print(uint8_t data)
{
    // Nombre de chiffres qui sont 
    // contenu dans le nombre à afficher
    uint8_t maxDigit;

    // Détermine le nombre de chiffre
    if (data > 100) maxDigit = 3;
    else if (data > 10) maxDigit = 2;
    else maxDigit = 1;

    // Transforme le nombre en une
    // table de charactère ASCII
    char strInt[NB_MAX_DIGIT] = {};
    for (uint8_t i = 0; i < maxDigit; i++){
        strInt[maxDigit-1-i] = char('0' + data % 10);
        data /= 10;
    }

    // Appel la fonction print avec la table
    // de charactère pour l'affichage avec RS232
    Debug::print(strInt);
}

// Fonction pour afficher un text suivi d'une valeur
// Est defined par DEBUG_PRINT
void Debug::print(const char* text, uint8_t data)
{
    // Appel les autres fonctions pour éviter 
    // la répétition de code
    Debug::print(text);
    Debug::print(data);
    Debug::print("\n");
}

