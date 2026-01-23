#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "enum_types.hpp"
#include "fonctions_generales.hpp"
#include "labyrinthe.hpp"
#include "terrain.hpp"



class WhatUSee : public sf::Drawable, public sf::Transformable {
	private: 
			sf::VertexArray m_vertices; //tableau des sommets pour dessiner la forme de vision

			void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
				 // Applique la transformation s'il y en a une (position, rotation, échelle)
			    states.transform *= getTransform();
			    // Dessine le tableau de sommets
			    target.draw(m_vertices, states);
			}


	public :
		WhatUSee(){
			m_vertices.setPrimitiveType(sf::TriangleFan);
		}

		void update(const std::vector<std::vector<int>>& maze, sf::Vector2f playerPos, int tileWidth) {
			
			/*charge les textures des blocs (tiles=tuiles) -> inutile si on fait des carrés rouges;
			if(!m_tileset.loadFromFile(tileset)){}*/
			
			m_vertices.clear();	//on vide le tableau

			std::vector<sf::Vector2f> sortedPoints=sortPoint(rayCasting(maze, playerPos, tileWidth), playerPos);

			if(sortedPoints.empty()) return;

			//scintillement de la torche pour un effet plus réaliste. on modifie la couleur à chaque Frame
			float flicker = (rand() % 10) / 100.f; // Variation de 0% à 10%
			
			//définition du centre (perso), commun à touts les triangles
			sf::Vertex centerVertex;
			centerVertex.position=playerPos;
			centerVertex.color = sf::Color(255,220,100);

			sf::Uint8 alpha = static_cast<sf::Uint8>(255.f * (0.9f + flicker));
			centerVertex.color=sf::Color(255, 220, 100, alpha);
			m_vertices.append(centerVertex);
			
			for(const auto& point : sortedPoints){
				sf::Vertex newVertex;
				newVertex.position = point;
				newVertex.color=sf::Color(255,220,100,0);
				m_vertices.append(newVertex);
			}

			sf::Vertex closingVertex;
		    closingVertex.position = sortedPoints[0];
		    closingVertex.color = sf::Color(255,220,100,0);
		    m_vertices.append(closingVertex);
		}
			
	std::vector<sf::Vector2f> rayCasting(const std::vector<std::vector<int>>& maze, sf::Vector2f playerPos, int tileWidth) {
		std::vector<sf::Vector2f>  proximateCorner = listProximateCorner(maze, playerPos,tileWidth);
		std::vector<sf::Vector2f> lightPoints;
		float smallAngle=0.0005f;
		    for (const auto &element : proximateCorner) {
				//Le coin du mur
		        float angle = atan2(element.y - playerPos.y, element.x - playerPos.x);
				lightPoints.push_back(getRayImpact(playerPos,angle, maze, tileWidth));

				//légèrement à gauche
				angle-=smallAngle;
				lightPoints.push_back(getRayImpact(playerPos,angle, maze, tileWidth));

				//légèrement à droite
				angle+=2*smallAngle;
				lightPoints.push_back(getRayImpact(playerPos,angle, maze, tileWidth));
		    }
		return lightPoints;
	}


	//Fonction qui liste les coins des murs à proximité du personnage
	std::vector<sf::Vector2f> listProximateCorner(const std::vector<std::vector<int>>& maze, sf::Vector2f playerPos, int tileWidth) {
	    std::vector<sf::Vector2f> returnArray;

	    // Rayon de recherche (en nombre de tuiles) autour du joueur
	    int distMax = 5; 
	    
		//case de la grille dans laquelle se situe le joueur
	    int playerI = static_cast<int>(playerPos.x / tileWidth);
	    int playerJ = static_cast<int>(playerPos.y / tileWidth);

	    // On parcourt les cases autour du joueur
	    for (int j = playerJ - distMax; j <= playerJ + distMax; ++j) {
	        for (int i = playerI - distMax; i <= playerI + distMax; ++i) {
	            
	            // Vérification des limites de la matrice
	            if (i < 0 || j < 0 || j >= maze.size() || i >= maze[0].size()) continue;

	            // Si c'est un mur (1), on ajoute ses 4 coins
	            if (maze[j][i] == 1) {
	                float x = i * tileWidth;
	                float y = j * tileWidth;

	                returnArray.push_back(sf::Vector2f(x, y));                       // Haut-Gauche
	                returnArray.push_back(sf::Vector2f(x + tileWidth, y));           // Haut-Droite
	                returnArray.push_back(sf::Vector2f(x + tileWidth, y + tileWidth));// Bas-Droite
	                returnArray.push_back(sf::Vector2f(x, y + tileWidth));           // Bas-Gauche
	            	}
	        	}
	   	 	}
	    	return returnArray;
		}
	
	//tri des points par angle :
	std::vector<sf::Vector2f> sortPoint(std::vector<sf::Vector2f> listOfPoint, sf::Vector2f playerPos){
		auto compareAngles = [playerPos](const sf::Vector2f& a, const sf::Vector2f& b) {
		    // Calcul de l'angle du point A par rapport au joueur
		    float angleA = std::atan2(a.y - playerPos.y, a.x - playerPos.x);
		    
		    // Calcul de l'angle du point B par rapport au joueur
		    float angleB = std::atan2(b.y - playerPos.y, b.x - playerPos.x);
		    
		    // On retourne vrai si A est "plus petit" (angle plus faible) que B
		    return angleA < angleB;
		};

		// 2. On l'utilise dans le tri
		std::sort(listOfPoint.begin(), listOfPoint.end(), compareAngles);
		return listOfPoint;
	}
};


