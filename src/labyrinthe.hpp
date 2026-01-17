#pragma once 

#include <vector>
#include <random>
#include <chrono>
#include <utility> // Pour std::pair
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "enum_types.hpp"

class grilleLabyrinthe {
	private:
	    // --- Attributs ---
	    int hauteur_;
	    int largeur_;
	    std::vector<std::vector<int>> grille_;
	    
	    // Générateur de nombres aléatoires
	    std::default_random_engine generateur_; 

	    // --- Constantes de la grille ---
	    const int MUR = 1;
	    const int CHEMIN = 0;

	    // --- Fonctions Privées (Aides à la génération) ---
	    
	    /**
	     * @brief Initialise la grille du labyrinthe entièrement à MUR (1).
	     */
	    void initialiser_grille();
	    
	    /**
	     * @brief Fonction récursive pour générer le labyrinthe en utilisant le DFS.
	     * @param ligne La ligne actuelle.
	     * @param col La colonne actuelle.
	     */
	    void dfs_generation(int ligne, int col);

	public:
	    // --- Constructeur ---
	    
	    /**
	     * @brief Construit un objet Labyrinthe et lance sa génération aléatoire.
	     * @param h Hauteur souhaitée (sera ajustée à la plus proche impaire si paire).
	     * @param l Largeur souhaitée (sera ajustée à la plus proche impaire si paire).
	     */
	    grilleLabyrinthe(int h, int l);
	    
	    // --- Méthodes Publiques ---
	    
	    /**
	     * @brief Affiche le labyrinthe dans la console.
	     */
	    void afficher() const;
	    
	    /**
	     * @brief Renvoie la matrice du labyrinthe.
	     */
	    const std::vector<std::vector<int>>& get_grille() const {
	        return grille_;
	    }
};
