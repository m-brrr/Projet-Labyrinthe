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
#include "terrain.hpp"
#include "gestion_son.hpp"

sf::Vector2f getRayImpact(sf::Vector2f playerPos, float angle, const std::vector<std::vector<int>>& maze, int tileWidth);

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
		float animationDelay=0.1;

		sf::Clock animation_clock;	//chaque personnage a sa propre horloge pour ses animations
		sf::Clock attack_clock;
		float attackDelay;
		float lastFootstepFrame=-1; //dernière frame avant 
		int soundVolume;
		int damageDisplay;	//sert à l'effet de rouge lorsque le perso prend un dégat


	
	public :
		//constucteur du personnage
		personnage(Direction o, std::string perso_choisi) :  maDirection(o), damageDisplay(0)
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

		bool isAlive () {
			return HealthPoint>0;
		}
		
		sf::Sprite& getSprite(){
			return character;
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

		void perso_animateMov(Son& regieSon){	//La fonction dépend de l'orientation
			if (animation_clock.getElapsedTime().asSeconds()>= animationDelay){	//getElapsedTime donne le tps écoulé depuis le dernier redémarrage
				postureAct= (postureAct+1)%8;
				animation_clock.restart();
		
            // Jouer le bruit de pas sur les frames 1 et 5 (quand le pied touche le sol)
            if ((postureAct == 1 || postureAct == 5) && lastFootstepFrame != postureAct) {
                regieSon.jouerSon(SoundEffectNames::BruitPas, soundVolume);// Volume réduit
                lastFootstepFrame = postureAct;
            }
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
			damageDisplay=20;			//Le visuel des dégats -> devient rouge
			HealthPoint-=(100*spellLevel);		//La perte de points de vie
		}

		bool getHealthPoints(){
			return HealthPoint;
		}
	};




class playerPerso : public personnage {
	private :
		PlayerBarreDeVie maBarreDeVie;

	public :

		playerPerso(Direction o, std::string perso_choisi) : personnage(o,perso_choisi), maBarreDeVie() {
			character.setPosition(400.f,300.f);
			soundVolume=20.f;
			attackDelay=0.1f;
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
			personnage::afficher_perso(window);
		}

		void afficher_barreVie(sf::RenderWindow &window){
			maBarreDeVie.afficherBarreDeVie(window);
		}

	};


class monster : public personnage {
	private :
		sf::Vector2f enemyAbsPos;
		sf::Vector2f inScreenPos;
		EnemyState theState;
		EnemyBarreDeVie barreVie;
		int transitionState=1; //1 pour en transition, 0 sinon
		float speed=100.f;
	public :

		monster(Direction o, std::string perso_choisi, sf::Vector2f enemyPosition) : personnage(o,perso_choisi), enemyAbsPos(enemyPosition), barreVie(enemyPosition) {
			character.setPosition(enemyAbsPos);
			theState=EnemyState::Patrol;
			soundVolume=0;
			attackDelay=0.5f;
		}

		void update(sf::Vector2f playerPos, const std::vector<std::vector<int>>& maze, float tileWidth, float dt, std::vector<std::unique_ptr<Spell>>& Spells,  Map &theMap, Son& regieSon){
			bool playerIsVisible = canSeePlayer(playerPos,maze, tileWidth );
			if(playerIsVisible) soundVolume=15;
			else soundVolume=0;
			
			switch (theState) {
            case EnemyState::Patrol:
                if (playerIsVisible) theState = EnemyState::Chase;
                else updatePatrol(dt, tileWidth, playerPos, theMap); 
                break;
			case EnemyState::Chase:
				if (playerIsVisible) updateChase(playerPos, dt,tileWidth, Spells, theMap);
				else theState=EnemyState::Patrol;
			}

			/*
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
			inScreenPos=enemyAbsPos-playerPos+sf::Vector2f(400.f, 300.f);
			character.setPosition(inScreenPos);
			barreVie.updatePosition(inScreenPos);
			perso_animateMov(regieSon);
		}

		void updatePatrol(float dt, float tileWidth, sf::Vector2f playerPos,  Map &theMap){
			sf::Vector2f posInGrid = sf::Vector2f(static_cast<int>(enemyAbsPos.x/tileWidth), static_cast<int>(enemyAbsPos.y/tileWidth));
			float distToSide=tileWidth/6;
			
			if ((enemyAbsPos.x-(posInGrid.x*tileWidth)>distToSide) && transitionState==1){
				setDirection(Direction::Left);
				if (theMap.canMove(character,enemyAbsPos+sf::Vector2f(-dt*speed,0.f),150.f)){
				enemyAbsPos+=sf::Vector2f(-dt*speed,0.f);}
			}
			else if ((enemyAbsPos.y-(posInGrid.y*tileWidth))>distToSide && transitionState==1){
				setDirection(Direction::Up);
				if (theMap.canMove(character,enemyAbsPos+sf::Vector2f(0.f, -dt*speed),150.f)){
				enemyAbsPos+=sf::Vector2f(0.f, -dt*speed);}
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

		void updateChase(sf::Vector2f playerAbsPos, float dt, float tileWidth, std::vector<std::unique_ptr<Spell>>& Spells, Map &theMap) {
		    sf::Vector2f diff = playerAbsPos - enemyAbsPos;
		    float distSq = diff.x * diff.x + diff.y * diff.y;
		    if (distSq < 1.0f) return; // si l'enemi est trop proche du joueur pour bouger

		    float tolerance = 5.0f;
		    float moveStep = speed * dt;

		    if (std::abs(diff.x) > tolerance && std::abs(diff.y) > tolerance) {
		        // On choisit l'axe où l'écart est le plus petit pour s'aligner
		        if (std::abs(diff.x) < std::abs(diff.y)) {
		            maDirection = (diff.x > 0) ? Direction::Right : Direction::Left;
		            // On s'assure de ne pas dépasser la cible (clamp)
		            float step = std::min(moveStep, std::abs(diff.x));
					float moveX=(diff.x > 0 ? 1 : -1) * step;
					if (theMap.canMove(character,enemyAbsPos+sf::Vector2f(moveX,0.f),150.f))enemyAbsPos.x += moveX;
		            
		        } 
		        else {
		            maDirection = (diff.y > 0) ? Direction::Down : Direction::Up;
		            float step = std::min(moveStep, std::abs(diff.y));
					float moveY=(diff.y > 0 ? 1 : -1) * step;
					if (theMap.canMove(character,enemyAbsPos+sf::Vector2f(0.f,moveY),150.f))enemyAbsPos.y += moveY;
		        }
		    } 
		    // si on est aligné
		    else {
		        maDirection = determinerDirection(diff.x, diff.y);
		        if (canAttack()) {
		            castASpell(Spells, maDirection, theMap);
		        }
		    }
		}

		bool canSeePlayer(sf::Vector2f playerAbsPos, const std::vector<std::vector<int>>& maze, int tileWidth) {
			const float PI = 3.14159265f;
		    sf::Vector2f diff = playerAbsPos - enemyAbsPos;
		    
		    // Calcul de la distance au carré
		    float distSqToPlayer = diff.x * diff.x + diff.y * diff.y;
		    
		    // Calcul de l'angle vers le joueur
		    float angleToPlayer = std::atan2(diff.y, diff.x);
		    
		    float seeAngle;
		    switch(maDirection) {
		        case Direction::Down  : seeAngle = PI / 2.0f;  break;
		        case Direction::Up    : seeAngle = -PI / 2.0f; break;
		        case Direction::Left  : seeAngle = PI;         break;
		        case Direction::Right : seeAngle = 0.0f;       break;
		    }
		    float angleDiff = angleToPlayer - seeAngle;
		    while (angleDiff > PI) angleDiff -= 2 * PI;
		    while (angleDiff < -PI) angleDiff += 2 * PI;

		    //Vérification du cône de vision (ici 60 degrés de chaque côté)
		    if (std::abs(angleDiff) < PI / 1.5f) {
		        sf::Vector2f impact = getRayImpact(enemyAbsPos, angleToPlayer, maze, tileWidth);
		        
		        sf::Vector2f impactDiff = impact - enemyAbsPos;
		        float distSqToImpact = impactDiff.x * impactDiff.x + impactDiff.y * impactDiff.y;
		        return distSqToImpact >= (distSqToPlayer - 0.5f); 	//la marge sert à éviter les bugs de collision
		    }

    return false;
}

		void setScreenPosition(sf::Vector2f LabyMov){
			character.setPosition(enemyAbsPos+ LabyMov);
		}

		void afficher_perso(sf::RenderWindow& window) override {
			personnage::afficher_perso(window);
			barreVie.afficherBarreDeVie(window);
		}
	
		void take_damage(int spellLevel) override {

			personnage::take_damage(spellLevel);

			if (HealthPoint>0) {
				float percentage = HealthPoint/HealthMax;
				std::cout<<percentage<<std::endl;
				barreVie.setPercentage(percentage);
			}
			else {
				barreVie.setPercentage(0.f);
				std::cout<<"C'est Perdu !"<<std::endl;
			}
		}
		
		void castASpell(std::vector<std::unique_ptr<Spell>>& Spells, Direction sens, Map& theMap) {
		    // On crée le sort et on l'ajoute directement à la liste du jeu
		    // On utilise *this pour que le sort sache qui l'a lancé
		    Spells.push_back(std::make_unique<Spell>(inScreenPos.x, inScreenPos.y, 1, sens, "RedFireBall", *this,theMap.getPosition()));
}
	};

