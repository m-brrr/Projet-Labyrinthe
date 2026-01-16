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