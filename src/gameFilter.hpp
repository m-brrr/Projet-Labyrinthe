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
		float initialSquareWidth; //taille carré initiale

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
			sf::Vector2f currentSize = square.getSize();
        	square.setSize(sf::Vector2f(initialSquareWidth * percentage, currentSize.y));
		}
};

class PlayerBarreDeVie : public BarreDeVie {
	public :
		PlayerBarreDeVie() : BarreDeVie() {

			spriteBarre.setOrigin(spriteBarre.getLocalBounds().width/2, spriteBarre.getLocalBounds().height/2);
			spriteBarre.setPosition(400.f,550.f);

			float scaleX = 4*spriteBarre.getLocalBounds().width;
			float scaleY = 1.5*spriteBarre.getLocalBounds().height;
			spriteBarre.setScale(4.f, 1.5);

			float recWidth=spriteBarre.getGlobalBounds().width-40.f;
			float recHeight=spriteBarre.getGlobalBounds().height-8.f;
			square.setSize(sf::Vector2f(recWidth,recHeight));
			initialSquareWidth=recWidth;

			square.setOrigin(0.f, square.getLocalBounds().height/ 2.f);
			float rectX = spriteBarre.getPosition().x - (spriteBarre.getGlobalBounds().width / 2.f) + 20.f;
			float rectY = spriteBarre.getPosition().y; // Même hauteur que le centre de la barre

			square.setPosition(rectX, rectY);
			square.setFillColor(sf::Color::Green);
		}
};

class EnemyBarreDeVie : public BarreDeVie {

	private :
		sf::Vector2f enemyRelPosition;	//position à l'écran
		float offsetAuDessus = 45.f; 

	public :
		EnemyBarreDeVie(sf::Vector2f charPosition) : BarreDeVie(), enemyRelPosition(charPosition) {
		
			float paddingX = 5.f;
			float paddingY = 3.f;
			
			sf::FloatRect b = spriteBarre.getLocalBounds();
			spriteBarre.setOrigin(b.width / 2.f, b.height / 2.f);
			spriteBarre.setPosition(enemyRelPosition.x, enemyRelPosition.y - offsetAuDessus);

			
			float internalWidth = b.width - (paddingX * 2.f);
			float internalHeight = b.height - (paddingY * 2.f);
			square.setSize(sf::Vector2f(internalWidth, internalHeight));
			initialSquareWidth=internalWidth;
			
			square.setOrigin(0.f, internalHeight / 2.f);
			float rectX = spriteBarre.getPosition().x - (b.width / 2.f) + paddingX;
			float rectY = spriteBarre.getPosition().y; // Même hauteur que le centre de la barre

			square.setPosition(rectX, rectY);
			square.setFillColor(sf::Color::Green);
		}

		void updatePosition(sf::Vector2f position){
			enemyRelPosition=position;
			spriteBarre.setPosition(enemyRelPosition.x,enemyRelPosition.y-offsetAuDessus);
			float rectX = spriteBarre.getPosition().x - (spriteBarre.getGlobalBounds().width / 2.f) + 5.f;
			float rectY = spriteBarre.getPosition().y; // Même hauteur que le centre de la barre

			square.setPosition(rectX, rectY);
		}
};
