#include "labyrinthe.hpp"
#include <iostream>
#include <algorithm> // Pour std::shuffle
#include <vector>

using namespace std;

// -------------------------------------------------------------------
// Constructeur
// -------------------------------------------------------------------

grilleLabyrinthe::grilleLabyrinthe(int h, int l) {
    // S'assurer que les dimensions sont impaires pour le DFS
    hauteur_ = (h % 2 == 0) ? h + 1 : h;
    largeur_ = (l % 2 == 0) ? l + 1 : l;
    
    // 1. Redimensionner et initialiser la grille
    grille_.resize(hauteur_, vector<int>(largeur_));
    initialiser_grille(); 

    // 2. Initialiser le générateur aléatoire
    generateur_.seed(chrono::system_clock::now().time_since_epoch().count());

    // 3. Lancer la génération DFS à partir d'une cellule intérieure (1, 1)
    dfs_generation(1, 1);
    
    // 4. Définir le point d'entrée et de sortie pour le jeu
    grille_[1][0] = CHEMIN; // Entrée à gauche
    grille_[hauteur_ - 2][largeur_ - 1] = CHEMIN; // Sortie à droite
}

// -------------------------------------------------------------------
// Initialisation
// -------------------------------------------------------------------

void grilleLabyrinthe::initialiser_grille() {
    for (int i = 0; i < hauteur_; ++i) {
        for (int j = 0; j < largeur_; ++j) {
            grille_[i][j] = MUR; 
        }
    }
}

// -------------------------------------------------------------------
// Algorithme de Génération (DFS Récursif)
// -------------------------------------------------------------------

void grilleLabyrinthe::dfs_generation(int ligne, int col) {
    grille_[ligne][col] = CHEMIN; // La cellule actuelle devient un chemin

    // Définir les 4 directions de mouvement (sauts de 2)
    vector<pair<int, int>> directions = {
        {0, 2},   // Droite
        {0, -2},  // Gauche
        {2, 0},   // Bas
        {-2, 0}   // Haut
    };

    // Mélanger aléatoirement l'ordre des directions (l'essence de l'aléatoire)
    shuffle(directions.begin(), directions.end(), generateur_);

    for (const auto& dir : directions) {
        int dl = dir.first;
        int dc = dir.second;

        int nouvelle_ligne = ligne + dl;
        int nouvelle_col = col + dc;

        // Vérifier si la nouvelle cellule est dans les limites et est un mur non visité
        if (nouvelle_ligne >= 1 && nouvelle_ligne < hauteur_ - 1 &&
            nouvelle_col >= 1 && nouvelle_col < largeur_ - 1 &&
            grille_[nouvelle_ligne][nouvelle_col] == MUR) {

            // 1. Casser le mur entre l'ancienne et la nouvelle cellule
            int mur_a_casser_ligne = ligne + dl/2;
            int mur_a_casser_col = col + dc/2;
            grille_[mur_a_casser_ligne][mur_a_casser_col] = CHEMIN;

            // 2. Continuer récursivement (Parcours en Profondeur)
            dfs_generation(nouvelle_ligne, nouvelle_col);
        }
    }
}

// -------------------------------------------------------------------
// Affichage
// -------------------------------------------------------------------

void grilleLabyrinthe::afficher() const {
    cout << "--- Labyrinthe Genere (" << hauteur_ << "x" << largeur_ << ") ---" << endl;
    for (int i = 0; i < hauteur_; ++i) {
        for (int j = 0; j < largeur_; ++j) {
            if (grille_[i][j] == MUR) {
                // Utiliser un caractère qui ressort bien, comme un bloc plein
                cout << "\u2588\u2588"; 
            } else {
                cout << "  "; // Deux espaces pour les chemins pour une meilleure proportion
            }
        }
        cout << endl;
    }
    cout << "------------------------------------------" << endl;
}