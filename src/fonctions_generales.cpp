#include "fonctions_generales.hpp"


sf::Vector2f getRayImpact(sf::Vector2f playerPos, float angle, const std::vector<std::vector<int>>& maze, int tileWidth) {
	    // Direction du rayon
	    sf::Vector2f rayDir(std::cos(angle), std::sin(angle));

		// Tous les calculs sont faits avec des cases de tailles normalisées à 1.
	    // Longueur du rayon pour sauter d'une ligne/colonne à l'autre (On évite la division par zéro avec un petit offset)
	    sf::Vector2f deltaDist(
	        std::abs(1.0f / (rayDir.x == 0 ? 1e-10f : rayDir.x)),
	        std::abs(1.0f / (rayDir.y == 0 ? 1e-10f : rayDir.y))
	    );

	    // Position du perso dans la matrice (static_cast pour convertir en entier de façon sécurité)
	    int mapX = static_cast<int>(playerPos.x / tileWidth);
	    int mapY = static_cast<int>(playerPos.y / tileWidth);

	    sf::Vector2f sideDist;
	    sf::Vector2i step;

	    // Calcul du pas (step) et de la distance du perso au bord de case le plus proche (sideDist)
	    if (rayDir.x < 0) {
	        step.x = -1;
	        sideDist.x = (playerPos.x / tileWidth - mapX) * deltaDist.x;
	    } else {
	        step.x = 1;
	        sideDist.x = (mapX + 1.0f - playerPos.x / tileWidth) * deltaDist.x;
	    }
	    if (rayDir.y < 0) {
	        step.y = -1;
	        sideDist.y = (playerPos.y / tileWidth - mapY) * deltaDist.y;
	    } else {
	        step.y = 1;
	        sideDist.y = (mapY + 1.0f - playerPos.y / tileWidth) * deltaDist.y;
	    }

	    // Boucle DDA : on avance jusqu'à toucher un mur (1)
	    bool hit = false;
	    float dist = 0;
	    int maxRange = 20; // Limite de portée pour éviter les boucles infinies
	    int steps = 0;

	    while (!hit && steps < maxRange) {
		    // Avancer dans la direction la plus proche
		    if (sideDist.x < sideDist.y) {
		        sideDist.x += deltaDist.x;
		        mapX += step.x;
		        dist = sideDist.x - deltaDist.x;  
		    } else {
		        sideDist.y += deltaDist.y;
		        mapY += step.y;
		        dist = sideDist.y - deltaDist.y;
		    }

		    // Vérifier les limites de la carte
		    if (mapX < 0 || mapY < 0 || mapY >= maze.size() || mapX >= maze[0].size()) {
		        // Hors limites = considéré comme un mur
		        hit = true;
		        break;
		    }
		    
		    // Vérifier si on a touché un mur
		    if (maze[mapY][mapX] == 1) {
		        hit = true;
		    }
		    
		    steps++;
		}

		// Si aucun mur trouvé, retourner un point très loin
		if (!hit) {
		    dist = maxRange;
		}
		//La lumière va comme ça illuminer 50% du mur, en fondu
		float wallPenetration=0.5f;
		dist+=wallPenetration;

	    // On retourne le point d'impact exact en pixels
	    return playerPos + (rayDir * (dist * (float)tileWidth));
	}