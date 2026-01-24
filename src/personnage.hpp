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
	protected :
		
		sf::Texture spriteSheet;
		sf::Sprite character;	
		std::string character_type;	//Le caractère voulu (à complêter avec la page de menu)

		std::array<std::array<sf::IntRect, 8>, 4> postures;
		int postureAct=0;	//de 0(repos) à 8
		Direction maDirection= Direction::Up;	 //0 face, 1 dos, 2 gauche, 3 droit
		
		std::string typeDeSpell="BlueFireBall";
		float HealthMax=1000;
		float HealthPoint = 1000;
		int Level=1;

		sf::Clock animation_clock;	//chaque personnage a sa propre horloge pour ses animations
		sf::Clock attack_clock;
		float animationDelay =0.1f;
		float attackDelay=0.3f;

		int damageDisplay=0;	//sert à l'effet de rouge lorsque le perso prend un dégat


	
	public :
		//constucteur du personnage
		personnage(Direction o, std::string perso_choisi) :  maDirection(o)
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

		}

		//renvoyer les infos du personnage

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

		virtual void afficher_perso(sf::RenderWindow &window) {
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

		virtual void take_damage(int spellLevel){
			damageDisplay=3;			//Le visuel des dégats -> devient rouge
			HealthPoint-=(100*spellLevel);		//La perte de points de vie
		}
};




class playerPerso : public personnage {
	private :
		PlayerBarreDeVie maBarreDeVie;

	public :

		playerPerso(Direction o, std::string perso_choisi, PlayerBarreDeVie barreVie) : personnage(o,perso_choisi), maBarreDeVie(barreVie) {
			character.setPosition(400.f,300.f);
		}

		void take_damage(int spellLevel) override {

			personnage::take_damage(spellLevel);

			if (HealthPoint>0) {
				float percentage = HealthPoint/HealthMax;
				std::cout<<percentage<<std::endl;
				maBarreDeVie.setPercentage(percentage);
			}
			else {
				maBarreDeVie.setPercentage(0.f);
				std::cout<<"C'est Perdu !"<<std::endl;
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
};


class monster : public personnage {
	private :
		sf::Vector2f enemyAbsPos;
		EnemyState theState;
		EnemyBarreDeVie barreVie;
		int transitionState=1; //1 pour en transition, 0 sinon
		float speed=100.f;
	public :

		monster(Direction o, std::string perso_choisi, sf::Vector2f enemyPosition) : personnage(o,perso_choisi), enemyAbsPos(enemyPosition), barreVie(enemyPosition) {
			character.setPosition(enemyAbsPos);
			theState=EnemyState::Patrol;
		}

		void update(sf::Vector2f playerPos, const std::vector<std::vector<int>>& maze, float tileWidth, float dt ){
			bool playerIsVisible = canSeePlayer(playerPos,maze, tileWidth );
			updatePatrol(dt, tileWidth, playerPos);
			
			/*switch (theState) {
            case EnemyState::Patrol:
                if (playerIsVisible) theState = EnemyState::Chase;
                else updatePatrol(dt, tileWidth); 
                break;

            case EnemyState::Chase:
                if (!playerIsVisible) theState = EnemyState::Search;
                else updateChase(playerPos, dt);
                break;
			
			
            case EnemyState::Search:
                if (playerIsVisible) theState = EnemyState::Chase;
                else updateSearch(dt); // L'ennemi attend ou retourne en patrouille après X secondes
                break;
			}
        	
			*/
			character.setPosition(enemyAbsPos-playerPos+sf::Vector2f(400.f, 300.f));
			perso_animateMov();
		}

		void updatePatrol(float dt, float tileWidth, sf::Vector2f playerPos){
			sf::Vector2f posInGrid = sf::Vector2f(static_cast<int>(enemyAbsPos.x/tileWidth), static_cast<int>(enemyAbsPos.y/tileWidth));
			float distToSide=tileWidth/6;
			if ((enemyAbsPos.x-(posInGrid.x*tileWidth)>distToSide) && transitionState==1){
				setDirection(Direction::Left);
				enemyAbsPos+=sf::Vector2f(-dt*speed,0.f);
			}
			else if ((enemyAbsPos.y-(posInGrid.y*tileWidth))>distToSide && transitionState==1){
				setDirection(Direction::Up);
				enemyAbsPos+=sf::Vector2f(0.f, -dt*speed);
			}
			else {
				transitionState=0;
				moveInSquare(tileWidth-2*distToSide, dt, posInGrid*tileWidth);
			}
		}

		void moveInSquare(float squareWidth, float dt, sf::Vector2f posInit) {
			
		    // Déplacement en fonction de la direction actuelle
		    switch (maDirection) {
		        case Direction::Right: // Droite
		            enemyAbsPos.x += speed*dt;
		            if (enemyAbsPos.x >= squareWidth+posInit.x) {
		                enemyAbsPos.x = squareWidth+posInit.x;
		                maDirection = Direction::Down; // Passer à bas
		            }
		            break;
		        case Direction::Down: // Bas
		            enemyAbsPos.y += speed*dt;
		            if (enemyAbsPos.y >= squareWidth+posInit.y) {
		                enemyAbsPos.y = squareWidth+posInit.y;
		                maDirection = Direction::Left; // Passer à gauche
		            }
		            break;
		        case Direction::Left: // Gauche
		            enemyAbsPos.x -= speed*dt;
		            if (enemyAbsPos.x <= posInit.x) {
		                enemyAbsPos.x = posInit.x;
		                maDirection = Direction::Up; // Passer à haut
		            }
		            break;
		        case Direction::Up: // Haut
		            enemyAbsPos.y -= speed*dt;
		            if (enemyAbsPos.y <= posInit.y) {
		                enemyAbsPos.y = posInit.y;
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

		bool canSeePlayer(sf::Vector2f playerAbsPos, const std::vector<std::vector<int>>& maze, int tileWidth ){
			float distanceToPlayer=std::sqrt(std::pow(playerAbsPos.x-enemyAbsPos.x,2)+std::pow(playerAbsPos.y-enemyAbsPos.y,2));
			float angle=std::atan2(playerAbsPos.y-enemyAbsPos.y,playerAbsPos.x-enemyAbsPos.x);

			sf::Vector2f impact=getRayImpact(enemyAbsPos, angle, maze, tileWidth);
			float distanceToImpact=std::sqrt(std::pow(impact.x-enemyAbsPos.x,2)+std::pow(impact.y-enemyAbsPos.y,2));
			return distanceToImpact>=distanceToPlayer;
		}

		void setScreenPosition(sf::Vector2f LabyMov){
			character.setPosition(enemyAbsPos+ LabyMov);
		}
	};

