#include <avr/io.h>
#include <util/delay.h>
#include "memoire_24.h"

#define DEL_VERTE ((0 << PA0) |(1 << PA1))  // Définir le bit pour la DEL verte
#define DEL_ROUGE ((1 << PA0) |(0 << PA1))  // Définir le bit pour la DEL rouge

const char texte[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*\0";
const uint16_t adresseMemoire = 0x0000;
Memoire24CXXX memoire;

void ecrireMemoire() {
    uint8_t longueur = sizeof(texte);
    memoire.ecriture(adresseMemoire, (uint8_t*)texte, longueur);
    _delay_ms(5 * longueur);  // Délai pour garantir l'écriture complète

}

bool lireEtComparer() {
    char texteLu[sizeof(texte)];
    memoire.lecture(adresseMemoire, (uint8_t*)texteLu, sizeof(texte));

    for (uint8_t i = 0; i < sizeof(texte); i++) {
        if (texteLu[i] != texte[i]) {
            return false;
        }
    }
    return true;
}

void afficherResultat(bool succes) {
    DDRA = (1 << PA0) | (1 << PA1); // Configurer les ports en sortie

    if (succes) {
        PORTA = DEL_VERTE;  // Allumer la DEL verte
    } else {
        PORTA = DEL_ROUGE;  // Allumer la DEL rouge
    }
}

int main() {
    memoire.init();  // Initialisation de la mémoire
    ecrireMemoire();  // Écriture de la chaîne en mémoire
    bool succes = lireEtComparer();  // Lecture et comparaison
    afficherResultat(succes);  // Afficher le résultat avec les DELs

    while (1);  // Boucle infinie pour garder l'affichage
}
