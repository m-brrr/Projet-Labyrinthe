#include "labyrinthe.h"
#include <iostream>

int main() {
    // Dimensions souhaitées (elles seront ajustées à impaires : 19x31)
    const int H = 18; 
    const int L = 30;

    std::cout << "Demarrage de la generation du labyrinthe aleatoire..." << std::endl;
    
    // Créer un objet Labyrinthe. La generation DFS est lancee ici !
    labyrinthe mon_laby(H, L); 

    // Afficher le labyrinthe généré
    mon_laby.afficher();

    // Exemple : Accéder a une cellule specifique pour votre jeu
    const auto& grille = mon_laby.get_grille();
    int statut_entree = grille[1][0]; // Devrait être 0 (Chemin)
    
    std::cout << "Le statut de la case (1, 0) est : " << statut_entree << std::endl;

    // Si vous voulez un nouveau labyrinthe, il suffit de creer un nouvel objet :
    // Labyrinthe autre_laby(25, 25);
    // autre_laby.afficher();

    return 0;
}