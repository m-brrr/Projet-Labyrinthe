#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "enum_types.hpp"
#include "labyrinthe.hpp"


class Map : public sf::Drawable, public sf::Transformable {
	private: 
			const std::vector<std::vector<int>> &maze;
			sf::VertexArray m_vertices; //tableau des sommets pour dessiner les tuiles
			sf::Texture m_tileset; //Texture contenant toutes les tuiles (inutile ici)
			float speed=500.f;

			sf::Texture textureDepart;
			sf::Sprite departSprite;

			sf::Texture textureArrivee;
			sf::Sprite arriveeSprite;
			sf::Clock exitGlowClock;


			void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
				 // Applique la transformation s'il y en a une (position, rotation, échelle)
			    states.transform *= getTransform();

			    // Applique la texture du tileset (ici aucun)
			    states.texture = &m_tileset;

			    // Dessine le tableau de sommets
			    target.draw(m_vertices, states);
			}

	public :
		Map(const std::vector<std::vector<int>> &laby) : maze(laby){
			this->setPosition(350.f,100.f);

			if (!textureDepart.loadFromFile("./assets/autre/depart.png")) {	//On vérifie que le chargement se soit bien passé
			        throw std::runtime_error("Échec du chargement de l'asset : ./assets/autre/depart.png");	//permet de lever une exception
			}
			
			if (!textureArrivee.loadFromFile("./assets/autre/arrivee.png")) {	//On vérifie que le chargement se soit bien passé
			        throw std::runtime_error("Échec du chargement de l'asset : ./assets/autre/arrivee.png" );	//permet de lever une exception
			}

			//Créer le spellSprite
				departSprite.setTexture(textureDepart);
				arriveeSprite.setTexture(textureArrivee);

			//La taille du departSprite est de 32x32

				departSprite.setScale(150.f/32.f, 150.f/32.f);
				departSprite.setRotation(90.f);

			//taille du spriteArrivee est de 20x32
				arriveeSprite.setScale(150.f/20.f, 150.f/32.f);
		}


		void load(sf::Vector2u tileSize, const std::vector<std::vector<int>> maze, 
				  unsigned int width, unsigned int height) {
			
			/*charge les textures des blocs (tiles=tuiles) -> inutile si on fait des carrés rouges;
			if(!m_tileset.loadFromFile(tileset)){}*/
			

			m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
			m_vertices.resize(width*height*6); //Les tuiles étant carrées, elles sont dessinées par 2 triangles, soit 6 sommets

			for (unsigned int i=0;i<width;i++){
				for (unsigned int j=0;j<height;j++) {
					
					//c'est le numéro de la tuile actuelle	(1 si c'est un mur et 0 sinon)
					const int tileNumber = maze[j][i];

					//c'est le pointeur vers les sommets des triangles de la tuile actuelle
					sf::Vertex* triangles = &m_vertices[(i+j*width)*6];

					// On définit les positions des 6 sommets de la tuile pour former le carré qu'on veut.
	                triangles[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
	                triangles[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
	                triangles[2].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
	                triangles[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
	                triangles[4].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
	                triangles[5].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);

					//ici on devrait définir la texture
					if (tileNumber==1){
						triangles[0].color = sf::Color::Red;
		                triangles[1].color = sf::Color::Red;
						triangles[2].color = sf::Color::Red;
						triangles[3].color = sf::Color::Red;
						triangles[4].color = sf::Color::Red;
						triangles[5].color = sf::Color::Red;
					}
					else {
						triangles[0].color = sf::Color::Black;
		                triangles[1].color = sf::Color::Black;
						triangles[2].color = sf::Color::Black;
						triangles[3].color = sf::Color::Black;
						triangles[4].color = sf::Color::Black;
						triangles[5].color = sf::Color::Black;
					}
					}
				}
			}

		void moveTheMap(float dt, Direction maDirection, float tileWidth) {
		    // Position fixe du joueur à l'écran
		    sf::Vector2f playerScreenPos(400.f, 300.f);
		    
		    // Calculer l'offset de déplacement (inverse car la carte bouge en sens opposé)
		    sf::Vector2f offset = getMov(dt, maDirection);  // ← Utilise ta fonction getMov
		    
		    // Nouvelle position de la carte
		    sf::Vector2f newMapPos = this->getPosition() + offset;
		    
		    // Position absolue du joueur dans le monde
		    sf::Vector2f playerAbsPos = playerScreenPos - newMapPos;
		    
		    // Vérifier si le mouvement est valide
		    if (canMovePlayer(playerAbsPos, tileWidth)) {  // ← Nouvelle fonction simplifiée
		        this->setPosition(newMapPos);
		    }
		}

		sf::Vector2f getMov(float deltaTime, Direction maDirection) const {
		    switch (maDirection) {
		        case Direction::Right: return sf::Vector2f(-speed * deltaTime, 0);
		        case Direction::Left:  return sf::Vector2f(speed * deltaTime, 0);
		        case Direction::Up:    return sf::Vector2f(0, speed * deltaTime);
		        case Direction::Down:  return sf::Vector2f(0, -speed * deltaTime);
		    }
		    throw std::invalid_argument("problème de direction");
		}

		// Nouvelle fonction pour vérifier la position du joueur (sans sprite en paramètre)
		bool canMovePlayer(sf::Vector2f playerAbsPos, float tileWidth) {
		    // Dimensions du joueur (à adapter selon ton sprite)
		    float width = 50.f;   // Largeur réelle du personnage
		    float height = 50.f;  // Hauteur de ton personnage
		    
		    std::vector<sf::Vector2f> pointsToCheck = {
		        {playerAbsPos.x - width/2, playerAbsPos.y - height/2}, // Coin haut-gauche
		        {playerAbsPos.x + width/2, playerAbsPos.y - height/2}, // Coin haut-droit
		        {playerAbsPos.x - width/2, playerAbsPos.y + height/2}, // Coin bas-gauche
		        {playerAbsPos.x + width/2, playerAbsPos.y + height/2}  // Coin bas-droit
		    };
		    
		    for (const auto& point : pointsToCheck) {
		        int col = static_cast<int>(point.x / tileWidth);
		        int row = static_cast<int>(point.y / tileWidth);
		        
		        // Vérifier les limites du tableau
		        if (row < 0 || row >= maze.size() || col < 0 || col >= maze[0].size()) {
		            return false;
		        }
		        
		        // Vérifier les murs
		        if (maze[row][col] == 1) {
		            return false;
		        }
		    }
		    
		    return true;
		}

		// Garde ton ancien canMove pour les autres usages (ennemis par exemple)
		bool canMove(const sf::Sprite& theSprite, sf::Vector2f absNewPosition, float tileWidth) {//prend la position absolue et renvoie s'il peut bouger
		    
		    float width = theSprite.getGlobalBounds().width;
		    float height = theSprite.getGlobalBounds().height;
		    
		    std::vector<sf::Vector2f> pointsToCheck = {
		        {absNewPosition.x - width/2, absNewPosition.y - height/2},
		        {absNewPosition.x + width/2, absNewPosition.y - height/2},
		        {absNewPosition.x - width/2, absNewPosition.y + height/2},
		        {absNewPosition.x + width/2, absNewPosition.y + height/2}
		    };
		    
		    for (const auto& point : pointsToCheck) {
		        int col = static_cast<int>(point.x / tileWidth);
		        int row = static_cast<int>(point.y / tileWidth);
		        
		        if (row < 0 || row >= maze.size() || col < 0 || col >= maze[0].size()) {
		            return false;
		        }
		        
		        if (maze[row][col] == 1) {
		            return false;
		        }
		    }
		    
		    return true;
		}

		void afficher_porte(sf::RenderWindow& window, int L, int H){
		    sf::Vector2f mapPos = this->getPosition();

		    departSprite.setPosition(mapPos.x, mapPos.y + 150.f); 
			sf::Vector2f exitPosWorld((L - 1) * 150.f, (H - 2) * 150.f);
		    arriveeSprite.setPosition(mapPos+exitPosWorld);
		    window.draw(departSprite);
			window.draw(arriveeSprite);
		}

		bool didWin(int H, int L, float tileWidth) {
		    // 1. Calculer où se trouve le joueur (centre écran) par rapport au (0,0) de la carte
		    // PositionEcran - PositionCarte = PositionDansLeLabyrinthe
		    sf::Vector2f playerScreenPos(400.f, 300.f);
		    sf::Vector2f playerAbsPos = playerScreenPos - this->getPosition();

		    // 2. Définir la zone de victoire (ex: la dernière colonne du labyrinthe)
		    float winThresholdX = (L - 1) * tileWidth; 
		    float winThresholdY = (H - 1) * tileWidth;

		    // 3. Condition : Le joueur a dépassé la limite vers la droite (ou le bas)
		    if (playerAbsPos.x > winThresholdX || playerAbsPos.y > winThresholdY) {
		        return true;
		    }

		    return false;
		}

		sf::Clock& getExitGlowClock(){
			return exitGlowClock;
		}
};