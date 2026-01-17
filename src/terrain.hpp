#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "enum_types.hpp"
#include "labyrinthe.hpp"


class Map : public sf::Drawable, public sf::Transformable {
	private: 
			sf::VertexArray m_vertices; //tableau des sommets pour dessiner les tuiles
			sf::Texture m_tileset; //Texture contenant toutes les tuiles (inutile ici)
			float speed=200.f;

			void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
				 // Applique la transformation s'il y en a une (position, rotation, échelle)
			    states.transform *= getTransform();

			    // Applique la texture du tileset (ici aucun)
			    states.texture = &m_tileset;

			    // Dessine le tableau de sommets
			    target.draw(m_vertices, states);
			}

	public :
		void load(sf::Vector2u tileSize, const std::vector<std::vector<int>> maze, 
				  unsigned int width, unsigned int height) {
			
			/*charge les textures des blocs (tiles=tuiles) -> inutile si on fait des carrés rouges;
			if(!m_tileset.loadFromFile(tileset)){}*/
			

			m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
			m_vertices.resize(width*height*6); //Les tuiles étant carrées, elles sont dessinées par 2 triangles, soit 6 sommets

			for (unsigned int i=0;i<width;i++){
				for (unsigned int j=0;j<height;j++) {
					
					//c'est le numéro de la tuile actuelle	(1 si c'est un mur et 0 sinon)
					const int tileNumber = maze[i][j];

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

			void moveTheMap(float deltaTime, Direction maDirection){
				switch (maDirection){
					case Direction::Right : this->move(-speed*deltaTime, 0); break;
					case Direction::Left : this->move(+speed*deltaTime, 0); break;
					case Direction::Up : this->move(0, speed*deltaTime); break;
					case Direction::Down : this->move(0, -speed*deltaTime); break;
				}
			}

			sf::Vector2f getMov(float deltaTime, Direction maDirection) const{
				switch (maDirection){
					case Direction::Right : return sf::Vector2f(-speed*deltaTime, 0);
					case Direction::Left : return sf::Vector2f(+speed*deltaTime, 0);
					case Direction::Up : return sf::Vector2f(0, speed*deltaTime);
					case Direction::Down : return sf::Vector2f(0, -speed*deltaTime);
				}
				throw std::invalid_argument("problème de direction");
			}
};