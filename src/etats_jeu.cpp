#include "etats_jeu.hpp"


void GameState::handleEvent() {	//méthode de gestion des entrées ponctuelles (à chaque Frame, c'est juste un parcourt rapide du buffer des évenemengts)
			 sf::Event event;
	        while (window.pollEvent(event)) {

	            if (event.type == sf::Event::Closed)
	                {
						window.close();
					}
				if(event.type==sf::Event::KeyPressed)
					{		
					//pour les attaques
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
						{	
							if(monPerso.canAttack()){
								Direction maDirection=monPerso.get_orientation();
								std::string typeDeSpell=monPerso.getSpellType();
								int Level=monPerso.getLevel();
								spells.push_back(std::make_unique<Spell>(0,0, Level, maDirection, typeDeSpell));	//On rajoute le sort à la liste des choses à afficher
							}
						}
					/*
					//pour l'interruption du jeu
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
						{
							machine.setState(std::make_unique<BreakState>(machine, window));	//on passe par un unique ptr pour éviter les problèmes de mémoire
						}
							*/
					}
	    		}
			}

void GameState::update(float dt)  {
    				//Pour les déplacements : 
					sf::Vector2f LabyMov;
				        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	
			            {
							monlaby.moveLabyrinthe(dt, Direction::Left);
							LabyMov=monlaby.getMov(dt, Direction::Left);

							monPerso.setDirection(Direction::Left);
							monPerso.perso_animateMov();
						}
				        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			            {
							monlaby.moveLabyrinthe(dt, Direction::Right);
							LabyMov=monlaby.getMov(dt, Direction::Right);

							monPerso.setDirection(Direction::Right);
							monPerso.perso_animateMov();
						}
				        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			            {
							monlaby.moveLabyrinthe(dt, Direction::Up);
							LabyMov=monlaby.getMov(dt, Direction::Up);

							monPerso.setDirection(Direction::Up);
							monPerso.perso_animateMov();
						}
				        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			            {
							monlaby.moveLabyrinthe(dt, Direction::Down);
							LabyMov=monlaby.getMov(dt, Direction::Down);

							monPerso.setDirection(Direction::Down);
							monPerso.perso_animateMov();
						}
					//Mettre à jour la position des sorts par rapport au personnage
					

						/*for (auto& pcharacter : allEnemies) {
			                if (pspell->didTouchCharacter(pcharacter->getBoundsCharacter())) {
			                    pcharacter->take_damage(pspell->getSpellLevel());
			                }
			            }*/

						for (auto it = spells.begin(); it != spells.end(); ) {
							(*it)->setPosition(dt, LabyMov);
							(*it)->Spell_animateMov();
						    if ((*it)->didTouchCharacter(monPerso.getBoundsCharacter())) {
						        std::cout << "YESSS" << std::endl;
						        monPerso.take_damage((*it)->getSpellLevel());
						        //it = spells.erase(it); // Supprime l'élément et met à jour l'itérateur
						    } else {
						        ++it; // Passe au sort suivant
						    }
						}
					}

void GameState::render() {		//méthode pour tout afficher
			window.clear();
			//window.draw(background);
			monlaby.drawLabyrinthe(window);
			monPerso.afficher_perso(window);
			for (const auto& pspell : spells) {  // 'spellPtr' est un std::unique_ptr<Spell>
    					pspell->afficherSort(window);            // Utilise '->' pour appeler la méthode
						}
			maBarre.afficherBarreDeVie(window);
			//window.draw(monsters);
			window.display();
		}
