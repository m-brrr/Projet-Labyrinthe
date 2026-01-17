#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "enum_types.hpp"


class labyrinthe {
	private : 
		sf::RectangleShape square;
		float speed;

	public :
		labyrinthe() : square((sf::Vector2f(50.f, 50.f))), speed(200.f) {
			square.setPosition(0, 0);
			square.setFillColor(sf::Color::Red);
			};

		void moveLabyrinthe(float deltaTime, Direction maDirection){
			switch (maDirection){
				case Direction::Right : square.move(-speed*deltaTime, 0); break;
				case Direction::Left : square.move(+speed*deltaTime, 0); break;
				case Direction::Up : square.move(0, speed*deltaTime); break;
				case Direction::Down : square.move(0, -speed*deltaTime); break;
			}
		}

		sf::Vector2f getMov(float deltaTime, Direction maDirection){
			switch (maDirection){
				case Direction::Right : return sf::Vector2f(-speed*deltaTime, 0);
				case Direction::Left : return sf::Vector2f(+speed*deltaTime, 0);
				case Direction::Up : return sf::Vector2f(0, speed*deltaTime);
				case Direction::Down : return sf::Vector2f(0, -speed*deltaTime);
			}
			throw std::invalid_argument("problème de direction");
		}

		sf::Vector2f getPosition(){
			return square.getPosition();
		}

		void drawLabyrinthe(sf::RenderWindow &window) const{
			window.draw(square);
		};
		
};

class Map : public sf::Drawable, public sf::Transformable {

	private: 
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
				 // Applique la transformation s'il y en a une (position, rotation, échelle)
			    states.transform *= getTransform();

			    // Applique la texture du tileset (ici aucun)
			    states.texture = &m_tileset;

			    // Dessine le tableau de sommets
			    target.draw(m_vertices, states);
			}

			sf::VertexArray m_vertices; //tableau des sommets pour dessiner les tuiles
			sf::Texture m_tileset; //Texture contenant toutes les tuiles (inutile ici)


	public :
		bool load(const std::string tileset, sf::Vector2u tileSize, const std::vector<std::vector<int>> maze, 
		unsigned int width, unsigned int height) {
			
			//charge les textures des blocs (tiles=tuiles) -> inutile si on fait des carrés rouges;
			if(!m_tileset.loadFromFile(tileset)){return false;}
			

			m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
			m_vertices.resize(width*height*6); //Les tuiles étant carrées, elles sont dessinées par 2 triangles, soit 6 sommets

			for (unsigned int i=0;i<width;i++){
				for (unsigned int j=0;j<height;j++) {
					
					//c'est le numéro de la tuile actuelle	(1 si c'est un mur et 0 sinon)
					const int tileNumber = maze[i+j*width];

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
					triangles[0].color = sf::Color::Red;
	                triangles[1].color = sf::Color::Red;
					triangles[2].color = sf::Color::Red;
					triangles[3].color = sf::Color::Red;
					triangles[4].color = sf::Color::Red;
					triangles[5].color = sf::Color::Red;

	              
				}
			}
			return true;
		}

	
};