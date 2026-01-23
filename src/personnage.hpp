#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <array>

#include "enum_types.hpp"
#include "fonctions_generales.hpp"
#include "sorts.hpp"

#include "gameFilter.hpp"



class personnage {
	private :
		
		sf::Texture spriteSheet;
		sf::Sprite character;	
		std::string character_type;	//Le caractère voulu (à complêter avec la page de menu)
		std::array<std::array<sf::IntRect, 8>, 4> postures;
		int postureAct=0;	//de 0(repos) à 8
		Direction maDirection= Direction::Up;	 //0 face, 1 dos, 2 gauche, 3 droit
		
		std::string typeDeSpell="BlueFireBall";
		float taille;
		float HealthMax=1000;
		float HealthPoint = 1000;
		int Level=1;

		sf::Clock animation_clock;	//chaque personnage a sa propre horloge pour ses animations
		sf::Clock attack_clock;
		float animationDelay =0.1f;
		float attackDelay=0.3f;

		BarreDeVie maBarreDeVie;
		int damageDisplay=0;


	
	public :
		//constucteur du personnage
		personnage(float t,Direction o, std::string perso_choisi, BarreDeVie barreVie) : taille(t), maDirection(o), maBarreDeVie(barreVie)
		{
			//On récupère la spriteSheet
				character_type=perso_choisi;
				std::string path = "./assets/texturePersonnage/" + perso_choisi + ".png";

				if (!spriteSheet.loadFromFile(path)) {	//On vérifie que le chargement se soit bien passé
			        throw std::runtime_error("Échec du chargement de l'asset : " + path);	//permet de lever une exception
			    }

			//Créer le personnage
				character.setTexture(spriteSheet);

			//La taille du personnage est de 77
				const int spriteWidth=77;
				const int spriteHeight=77;

			//On les remplit 
				for(int j=0;j<8;j++){
					for (int i=0;i<4;i++) {
						sf::IntRect rectFace_elt(spriteWidth*j,spriteHeight*i, spriteWidth,spriteHeight);	//Face
						postures[i][j]=rectFace_elt;
					}
				}
			//On positionne le centre du personnage sur le centre de l'écran
				character.setTextureRect(postures[0][0]);
				character.setOrigin(character.getLocalBounds().width / 2.f, character.getLocalBounds().height / 2.f);
				character.setPosition(400.f, 300.f);
				

		}

		//renvoyer les infos du personnage
		float get_taille() const {
			return taille;
		}

		sf::FloatRect getBoundsCharacter() const {
        	return character.getGlobalBounds();
    	}

		Direction get_orientation() const {
			return maDirection;
		}

		std::string getSpellType() const {
			return typeDeSpell;
		}

		int getLevel() const {
			return Level;
		}

		//Méthodes utiles (non const)

		bool canAttack()
		{
			if(attack_clock.getElapsedTime().asSeconds()>=attackDelay){
				attack_clock.restart();
				return true;
			}
			return false;
		}

		void setDirection(Direction newDirection){
			maDirection=newDirection;
		}

		void perso_animateMov(){	//La fonction dépend de l'orientation
			if (animation_clock.getElapsedTime().asSeconds()>= animationDelay){	//getElapsedTime donne le tps écoulé depuis le dernier redémarrage
				postureAct= (postureAct+1)%8;
				animation_clock.restart();
			}
		}

		void afficher_perso(sf::RenderWindow &window) {
			int i=convertToNumberCharacter(maDirection);
			character.setTextureRect(postures[i][postureAct]);

			//On vérifie qu'il ne soit pas en train de prendre des dégats
			if (damageDisplay>0) {
				sf::Color rouge(255, 100, 100, 255); // Rouge clair (R, G, B, Alpha)
				character.setColor(rouge);
				damageDisplay-=1;
			}

			window.draw(character);
			character.setColor(sf::Color::White); // Réinitialise la couleur pour enlever les filtres

		}
		
		void take_damage(int spellLevel){
			damageDisplay=3;			//Le visuel des dégats -> devient rouge
			HealthPoint-=(100*spellLevel);		//La perte de points de vie

			if (HealthPoint>0) {
				float percentage = HealthPoint/HealthMax;
				std::cout<<percentage<<std::endl;
				maBarreDeVie.takeDamage(percentage);
			}
			else {
				std::cout<<"C'est Perdu !"<<std::endl;
			}
		}
};

class monster : public personnage {
	private :
		sf::Vector2f enemyAbsPos;
		EnemyState theState= EnemyState::Patrol;
		float speed=200.f;
	public :
		bool canSeePlayer(sf::Vector2f playerAbsPos, const std::vector<std::vector<int>>& maze, int tileWidth ){
			float distanceToPlayer=std::sqrt(std::pow(playerAbsPos.x-enemyAbsPos.x,2)+std::pow(playerAbsPos.y-enemyAbsPos.y,2));
			float angle=std::atan2(playerAbsPos.y-enemyAbsPos.y,playerAbsPos.x-enemyAbsPos.x);

			sf::Vector2f impact=getRayImpact(enemyAbsPos, angle, maze, tileWidth);
			float distanceToImpact=std::sqrt(std::pow(impact.x-enemyAbsPos.x,2)+std::pow(impact.y-enemyAbsPos.y,2));
			return distanceToImpact>=distanceToPlayer;
		}

		void update(sf::Vector2f playerPos, const std::vector<std::vector<int>>& maze, int tileWidth, float dt ){
			bool playerIsVisible = canSeePlayer(playerPos,maze, tileWidth );
			switch (theState) {
            case EnemyState::Patrol:
                if (playerIsVisible) theState = EnemyState::Chase;
                else updatePatrol(dt, tileWidth); 
                break;

            case EnemyState::Chase:
                if (!playerIsVisible) theState = EnemyState::Search;
                else updateChase(playerPos, dt);
                break;
			
			/*
            case EnemyState::Search:
                if (playerIsVisible) theState = EnemyState::Chase;
                else updateSearch(dt); // L'ennemi attend ou retourne en patrouille après X secondes
                break;
        	
			*/
			}
		}

		void updatePatrol(float dt, float tileWidth){
			sf::Vector2i posInGrid = sf::Vector2i(static_cast<int>(enemyAbsPos.x/tileWidth), static_cast<int>(enemyAbsPos.y/tileWidth));
			float distToSide=tileWidth/6;
			if (enemyAbsPos.x-(posInGrid.x*tileWidth)>distToSide){
				enemyAbsPos+=sf::Vector2f(-dt*speed,0.f);
			}
			else if (enemyAbsPos.y-(posInGrid.y*tileWidth)>distToSide){
				enemyAbsPos+=sf::Vector2f(0.f, -dt*speed);
			}
			else {
				moveInSquare(tileWidth-2*distToSide, dt);
			}
		}

		void moveInSquare(float squareWidth, float dt) {
			Direction maDirection = Direction::Right;
		    // Déplacement en fonction de la direction actuelle
		    switch (maDirection) {
		        case Direction::Right: // Droite
		            enemyAbsPos.x += speed*dt;
		            if (enemyAbsPos.x >= squareWidth) {
		                enemyAbsPos.x = squareWidth;
		                maDirection = Direction::Down; // Passer à bas
		            }
		            break;
		        case Direction::Down: // Bas
		            enemyAbsPos.y += speed*dt;
		            if (enemyAbsPos.y >= squareWidth) {
		                enemyAbsPos.y = squareWidth;
		                maDirection = Direction::Left; // Passer à gauche
		            }
		            break;
		        case Direction::Left: // Gauche
		            enemyAbsPos.x -= speed*dt;
		            if (enemyAbsPos.x <= 0) {
		                enemyAbsPos.x = 0;
		                maDirection = Direction::Up; // Passer à haut
		            }
		            break;
		        case Direction::Up: // Haut
		            enemyAbsPos.y -= speed*dt;
		            if (enemyAbsPos.y <= 0) {
		                enemyAbsPos.y = 0;
		                maDirection = Direction::Right; // Revenir à droite
		            }
		            break;
		   		}
			}

		void updateChase(sf::Vector2f playerAbsPos, float dt){
			sf::Vector2f direction=sf::Vector2f(playerAbsPos.x-enemyAbsPos.x, playerAbsPos.y-enemyAbsPos.y);
			float distanceToPlayer=std::sqrt(std::pow(playerAbsPos.x-enemyAbsPos.x,2)+std::pow(playerAbsPos.y-enemyAbsPos.y,2));
			sf::Vector2f normalizedDirection= direction*distanceToPlayer;

			enemyAbsPos+=speed*dt*normalizedDirection;
		}
};

