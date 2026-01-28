Labyrinthe - Jeu d'Aventure SFML
Bienvenue dans Labyrinthe, un jeu d'exploration et de combat développé en C++ avec la bibliothèque SFML. Plongé dans un labyrinthe généré aléatoirement, vous devrez trouver la sortie tout en survivant aux ennemis qui rôdent dans l'obscurité.

----------------------------------------------------------------------------------------------------------------
Fonctionnalités principales

Génération Procédurale : Chaque labyrinthe est unique, généré par un algorithme de parcours en profondeur (DFS).

Système de Vision (Raycasting) : Un effet de lumière dynamique simule ce que le personnage voit réellement à travers les murs.

Machine à États : Navigation fluide entre le menu principal, la sélection de personnage, les réglages, le jeu et les écrans de fin.

Combats & Sorts : Système de tir de projectiles (Fireballs) avec gestion de collision et barres de vie.

Personnalisation : Choix entre plusieurs classes de personnages (Guerrier, Princesse, Enfant, Cowboy).

Ambiance Sonore : Musiques et effets sonores spatialisés.

-------------------------------------------------------------------------------------------------------------------
Compilation et Installation
Prérequis
Vous devez avoir installé la bibliothèque SFML (version 2.5 ou +) sur votre système.

Sur Ubuntu : sudo apt-get install libsfml-dev

--------------------------------
Compilation
Un fichier CMake est fourni à la racine. Pour compiler le projet, ouvrez un terminal et tapez :

mkdir build && cd build
cmake ..
make

NB : il est parfois nécessaire de faire cd .. avant de pouvoir compiler, car les fichiers ne sont étonnement pas construits dans le build
---------------------
Lancement
Une fois la compilation terminée, lancez l'exécutable :

./projet_labyrinthe

----------------------------

Commandes de jeu
Flèches : pour Se déplacer
Espace : pour lancer un sort d'attaque
Échap : pour Mettre le jeu en pause
Souris : Naviguer dans les menus

-----------------------------

Amusez vous !