#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <array>
#include "enum_types.hpp"


class BarreDeVie {
	private :
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

				spriteBarre.setPosition(200.f,550.f);
				spriteBarre.setScale(4.f, 1.5);

				float recWidth=spriteBarre.getGlobalBounds().width-40.f;
				float recHeight=spriteBarre.getGlobalBounds().height-10.f;

				sf::RectangleShape rectangle(sf::Vector2f(recWidth, recHeight));

				square=rectangle;

				square.setPosition(220.f,555.f);


				square.setFillColor(sf::Color::Green);
				
		}

		void afficherBarreDeVie(sf::RenderWindow &window){
			window.draw(spriteBarre);
			window.draw(square);
		}

		void takeDamage(float percentage){	//vie restante en pourcentage de ta vie maximale
			square.setScale(percentage, 1);
		}

};
