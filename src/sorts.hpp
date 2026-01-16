#pragma once
#include "enum_types.hpp"
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Spell {
	protected :
		int posInitX;	//Ces deux champs ne seront utiles que lorsqu'il y aura des IA
		int posInitY;

		int posActX;	//position relative au perso
		int posActY;

		int spellLevel=1;
		float speed = 400*spellLevel; //à modifier avec le niveau du joueur, ulterieurement
		
		Direction maDirection;
		// SpellType monSpell;    Sera utile lorsqu'on aura réussi à transformer les enum en string. Avec un stringify ?
		std::string SpellChosen;
		sf::Texture spriteSheet;
		sf::Sprite theSpellSprite;	

		//Horloges
		int postureAnimation=0;
		sf::IntRect animationArray[6];
		sf::Clock animation_clock;
		float animationDelay=0.1f;

	public :
		Spell(int x, int y, int Level, Direction dir, std::string typeDeSpell) : posInitX(x), posInitY(y), spellLevel(Level), posActX(x), posActY(y), maDirection(dir) 
		{
			SpellChosen=typeDeSpell;
			std::string path = "./assets/spells/" + SpellChosen + ".png";

			if (!spriteSheet.loadFromFile(path)) {	//On vérifie que le chargement se soit bien passé
			        throw std::runtime_error("Échec du chargement de l'asset : " + path);	//permet de lever une exception
			}
			
			//Créer le spellSprite
				theSpellSprite.setTexture(spriteSheet);

			//La taille du spellSprite est de 512x197
				const int spriteWidth=512;
				const int spriteHeight=197;

			//On les remplit 
				for(int j=0;j<3;j++){
					for (int i=0;i<2;i++) {
						sf::IntRect rectFace_elt(spriteWidth*j,spriteHeight*i, spriteWidth,spriteHeight);	//Face
						animationArray[j*2+i]=rectFace_elt;
					}
				}

			//On positionne le sort sur l'écran
				theSpellSprite.setPosition(spellProduction(maDirection));
				theSpellSprite.setScale(0.1,0.1);
				theSpellSprite.setRotation(90.f*(convertToNumberSpell(maDirection)+3));
		}

		~Spell() { std::cout << "Destructeur appelé pour le sort !" << std::endl; }

		sf::Vector2f getCenterSpell() {
				    return sf::Vector2f(theSpellSprite.getPosition().x + theSpellSprite.getGlobalBounds().width / 2.f,
				    theSpellSprite.getPosition().y + theSpellSprite.getGlobalBounds().height / 2.f);
		}

		int getSpellLevel() const{
			return spellLevel;
		}

		void setPosition(float dt,sf::Vector2f LabyMov){	//Position relative au personnage, qui est au centre de l'écran
			switch (maDirection){
				case Direction::Right : theSpellSprite.move(sf::Vector2f(speed*dt, 0)+LabyMov); break;; break;
				case Direction::Left : theSpellSprite.move(sf::Vector2f(-speed*dt, 0)+LabyMov); break;
				case Direction::Up : theSpellSprite.move(sf::Vector2f(0, -speed*dt)+LabyMov); break;
				case Direction::Down : theSpellSprite.move(sf::Vector2f(0, +speed*dt)+LabyMov); break;
			}
		}
		
		void Spell_animateMov(){	//La fonction dépend de l'orientation
			if (animation_clock.getElapsedTime().asSeconds()>= animationDelay){	//getElapsedTime donne le tps écoulé depuis le dernier redémarrage
				postureAnimation= (postureAnimation+1)%6;
				animation_clock.restart();
			}
		}

		void afficherSort(sf::RenderWindow &window){
			theSpellSprite.setTextureRect(animationArray[postureAnimation]);
			window.draw(theSpellSprite);
		}

		bool didTouchCharacter(const sf::FloatRect& boundsPersonnage) const {
        	return theSpellSprite.getGlobalBounds().intersects(boundsPersonnage);
    	}

		bool isOutOfScreen(float windowWidth, float windowHeight) const {
        sf::FloatRect bounds = theSpellSprite.getGlobalBounds();
        return bounds.top + bounds.height < 0 ||  // Sorti par le haut
               bounds.top > windowHeight ||        // Sorti par le bas
               bounds.left + bounds.width < 0 ||   // Sorti par la gauche
               bounds.left > windowWidth;          // Sorti par la droite
    }

};

