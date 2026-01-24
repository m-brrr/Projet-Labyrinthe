#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <array>
#include "enum_types.hpp"


class BarreDeVie {
	protected :
		int healthPoints;
		sf::Texture textureSheet;
		sf::Sprite spriteBarre;
		sf::RectangleShape square;
		int life; //En % de la vie totale

	public : 
		BarreDeVie() : healthPoints(100), life(100){
			std::string path = "./assets/autre/BarreDeVie.png";

			if (!textureSheet.loadFromFile(path)) {	//On vérifie que le chargement se soit bien passé
			        throw std::runtime_error("Échec du chargement de l'asset : " + path);	//permet de lever une exception
			}
			
			//Créer le spellSprite
				spriteBarre.setTexture(textureSheet);

			//La taille du spellSprite est de 106x10
				const int spriteWidth=106;
				const int spriteHeight=10;
				
		}

		void afficherBarreDeVie(sf::RenderWindow &window){
			window.draw(spriteBarre);
			window.draw(square);
		}

		void setPercentage(float percentage){	//vie restante en pourcentage de ta vie maximale
			square.setScale(percentage, 1);
		}
};

class PlayerBarreDeVie : public BarreDeVie {
	public :
		PlayerBarreDeVie() : BarreDeVie() {
			spriteBarre.setPosition(200.f,550.f);
			spriteBarre.setScale(4.f, 1.5);
			
			float recWidth=spriteBarre.getGlobalBounds().width-40.f;
			float recHeight=spriteBarre.getGlobalBounds().height-10.f;

			sf::RectangleShape rectangle(sf::Vector2f(recWidth, recHeight));

			square=rectangle;

			square.setPosition(220.f,555.f);
			square.setFillColor(sf::Color::Green);
		}
};

class EnemyBarreDeVie : public BarreDeVie {

	private :
		sf::Vector2f enemyRelPosition;

	public :
		EnemyBarreDeVie(sf::Vector2f Position) : BarreDeVie(), enemyRelPosition(Position) {
				spriteBarre.setPosition(Position);
				spriteBarre.setScale(1.f, 1.f);	//à régler

				float recWidth=spriteBarre.getGlobalBounds().width-10.f;	//à régler
				float recHeight=spriteBarre.getGlobalBounds().height-4.f;	//à régler

				sf::RectangleShape rectangle(sf::Vector2f(recWidth, recHeight));

				square=rectangle;

				square.setPosition(enemyRelPosition+sf::Vector2f(5.f,3.f));
				square.setFillColor(sf::Color::Green);
		}
};
