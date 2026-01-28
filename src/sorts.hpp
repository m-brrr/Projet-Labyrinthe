#pragma once
#include "enum_types.hpp"
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

class personnage;

class Spell {
	protected :
		int posInitX;	//Ces deux champs ne seront utiles que lorsqu'il y aura des IA
		int posInitY;

		int posActX;	//position relative au perso
		int posActY;

		sf::Vector2f posAbsSpell;

		int spellLevel=1;
		float speed = 400*spellLevel; //à modifier avec le niveau du joueur, ulterieurement
		const personnage& emetteur;	

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
		Spell(int x, int y, int Level, Direction dir, std::string typeDeSpell, const personnage& theEmittor, sf::Vector2f mapPos) : posInitX(x), posInitY(y), spellLevel(Level), posActX(x), posActY(y), maDirection(dir), emetteur(theEmittor)
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
				theSpellSprite.setTextureRect(animationArray[0]);
				theSpellSprite.setOrigin(theSpellSprite.getLocalBounds().width / 2.f, theSpellSprite.getLocalBounds().height / 2.f);
				theSpellSprite.setScale(0.1,0.1);

				theSpellSprite.setRotation(90.f*(convertToNumberSpell(maDirection)+3));	//Rotation par rapport à l'origine (ie au centre du spell)
				theSpellSprite.setPosition(posActX,posActY);
				posAbsSpell = sf::Vector2f(0.f,0.f);

				posAbsSpell = theSpellSprite.getPosition()- mapPos;
			}

		~Spell() { std::cout << "Destructeur appelé pour le sort !" << std::endl; }

		sf::Vector2f getCenterSpell() {
				    return sf::Vector2f(theSpellSprite.getPosition().x + theSpellSprite.getGlobalBounds().width / 2.f,
				    theSpellSprite.getPosition().y + theSpellSprite.getGlobalBounds().height / 2.f);
		}

		int getSpellLevel() const{
			return spellLevel;
		}

		const personnage* getEmittor() const {
			return &emetteur;
		}

		sf::Sprite*  getSprite(){
			return &theSpellSprite;
		}

		bool setPosition(float dt, Map &theMap, float tileWidth) {
		    // 1. Calculer la future position absolue (dans le labyrinthe)
		    sf::Vector2f nextAbsPos = getNewPosition(dt, posAbsSpell, speed, maDirection);

		    // 2. Vérifier si cette position absolue tape un mur
		    if (theMap.canMove(theSpellSprite, nextAbsPos, tileWidth)) { 
		        // 3. Mettre à jour la position logique
		        posAbsSpell = nextAbsPos;

		        // 4. Mettre à jour la position RÉELLE à l'écran pour l'affichage
		        // Position Écran = Position Absolue + Position de la Map
		        theSpellSprite.setPosition(posAbsSpell + theMap.getPosition());
		        
		        return true;
		    }
    return false;
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

		bool didTouchCharacter(const sf::FloatRect& boundsPersonnage, const personnage& victime) const {
			
			if (&victime!=&emetteur) return theSpellSprite.getGlobalBounds().intersects(boundsPersonnage);	//on ne touche que si ce n'est pas notre sort
			else return false;
    	}

		bool isOutOfScreen(float windowWidth, float windowHeight) const {
        sf::FloatRect bounds = theSpellSprite.getGlobalBounds();
        return bounds.top + bounds.height < 0 ||  // Sorti par le haut
               bounds.top > windowHeight ||        // Sorti par le bas
               bounds.left + bounds.width < 0 ||   // Sorti par la gauche
               bounds.left > windowWidth;          // Sorti par la droite
    }

};

