#include "etats_jeu.hpp"

		//Page de Jeu

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
						if (event.key.code == sf::Keyboard::Space) 
						{	
							if(monPerso.canAttack()){
								Direction maDirection=monPerso.get_orientation();
								std::string typeDeSpell=monPerso.getSpellType();
								int Level=monPerso.getLevel();
								spells.push_back(std::make_unique<Spell>(400,300, Level, maDirection, typeDeSpell, monPerso));	//On rajoute le sort à la liste des choses à afficher
							}
						}
					
					//pour l'interruption du jeu
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
						{
							std::cout<<"Mise en Pause"<<std::endl;
							machine.addState(StatesNames::Pause, std::move(std::make_unique<BreakState>(machine, window, regieSon)));
                			machine.changeStateRequest(StatesNames::Pause);
						}
							
					}
	    		}
			}

void GameState::update(float dt)  {
    				//Pour les déplacements : 
					sf::Vector2f LabyMov=sf::Vector2f(0.f,0.f);
				        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	
			            {
							theMap.moveTheMap(dt, Direction::Left);
							LabyMov=theMap.getMov(dt, Direction::Left);

							monPerso.setDirection(Direction::Left);
							monPerso.perso_animateMov(regieSon);
						}
				        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			            {
							theMap.moveTheMap(dt, Direction::Right);
							LabyMov=theMap.getMov(dt, Direction::Right);

							monPerso.setDirection(Direction::Right);
							monPerso.perso_animateMov(regieSon);
						}
				        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			            {
							theMap.moveTheMap(dt, Direction::Up);
							LabyMov=theMap.getMov(dt, Direction::Up);

							monPerso.setDirection(Direction::Up);
							monPerso.perso_animateMov(regieSon);
						}
				        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			            {
							theMap.moveTheMap(dt, Direction::Down);
							LabyMov=theMap.getMov(dt, Direction::Down);

							monPerso.setDirection(Direction::Down);
							monPerso.perso_animateMov(regieSon);
						}
					//Mettre à jour la position des sorts par rapport au personnage
					

						/*for (auto& pcharacter : allEnemies) {
			                if (pspell->didTouchCharacter(pcharacter->getBoundsCharacter())) {
			                    pcharacter->take_damage(pspell->getSpellLevel());
			                }
			            }*/
					 
					//Mise à jour de la position du perso dans la map (absolue)
						sf::Vector2f mapPos=theMap.getPosition();
						sf::Vector2f playerPos= sf::Vector2f(400.f,300.f)-mapPos;
						
					//Mise à jour des enemis
						for (auto it = allEnemies.begin(); it != allEnemies.end(); ) {
							(*it)->update(playerPos,grilleLaby.get_grille(),150.f,dt, spells, theMap, regieSon);
							
							++it; // Passe au monstre suivant
						}

					//Mise à jour des sorts
						for (auto it = spells.begin(); it != spells.end(); ) {

							(*it)->setPosition(dt, LabyMov);
							(*it)->Spell_animateMov();
							bool spellDestroyed = false;

						    if ((*it)->didTouchCharacter(monPerso.getBoundsCharacter(),monPerso)) {
						        std::cout << "YESSS" << std::endl;
						        monPerso.take_damage((*it)->getSpellLevel());
						        spellDestroyed = true; // Supprime l'élément et met à jour l'itérateur
						    } 

							if (!spellDestroyed) {
						        for (auto& enemy : allEnemies) {
						            if ((*it)->didTouchCharacter(enemy->getBoundsCharacter(),*enemy)) {
						                std::cout << "Ennemi touché !" << std::endl;
						                enemy->take_damage((*it)->getSpellLevel());
						                spellDestroyed = true;
						                break; // Le sort est consommé par le premier ennemi touché
						            }
						        }
							}

							if (spellDestroyed) {
								it = spells.erase(it);
							}
							else {it++;}
						}
						
						for (auto enemyIt = allEnemies.begin(); enemyIt != allEnemies.end(); ) {
						    if (!(*enemyIt)->isAlive()) {
						        std::cout << "Un ennemi vient de mourir !" << std::endl;
						        enemyIt = allEnemies.erase(enemyIt);
						    } else {
						        ++enemyIt;
						    }
						}
						//mettre à jour la vue (ie le rayCasting)
						if (monPerso.getHealthPoints()<=0){
							std::cout<<"You Lost"<<std::endl;
							machine.addState(StatesNames::GameOver, std::move(std::make_unique<EndState>(machine, window, regieSon)));
                			machine.changeStateRequest(StatesNames::GameOver);
						}
						myView.update(grilleLaby.get_grille(),playerPos, 150);
						myView.setPosition(mapPos);	
}


void GameState::render() {		//méthode pour tout afficher

			lightmap.clear(sf::Color(10,10,20));//quasi noir
			lightmap.draw(myView,sf::BlendAdd);
			lightmap.display();

			window.clear();
			window.draw(theMap);

			monPerso.afficher_perso(window);
			for (const auto& pmonster : allEnemies){
				pmonster->afficher_perso(window);
			}
			
			lightSprite.setTexture(lightmap.getTexture());
			window.draw(lightSprite, sf::BlendMultiply);

			for (const auto& pspell : spells) {  // 'spellPtr' est un std::unique_ptr<Spell>
    					pspell->afficherSort(window);            // Utilise '->' pour appeler la méthode
						}
			
			monPerso.afficher_barreVie(window);
			window.display();
		}


		//Page de Fin de Jeu
		

void EndState::handleEvent() {
	sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();

        // Interaction avec les boutons
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (titreFin.RetryIsPressed(mousePos)) {
				regieSon.jouerSon(SoundEffectNames::Click, 100.f);
				machine.addState(StatesNames::Game, std::move(std::make_unique<GameState>(machine, window, regieSon)));
                machine.changeStateRequest(StatesNames::Game);
            }
            if (titreFin.MenuIsPressed(mousePos)) {
				regieSon.jouerSon(SoundEffectNames::Click, 100.f);
                machine.addState(StatesNames::Menu, std::move(std::make_unique<MenuState>(machine, window, regieSon)));
                machine.changeStateRequest(StatesNames::Menu);
            }
        }
    }
}	

void EndState::update(float dt) {
	sf::Vector2i posSouris = sf::Mouse::getPosition(window);
    titreFin.update(dt, posSouris);
}

void EndState::render() {
	window.clear();
	titreFin.afficherTitre(window);
	window.display();
}

		//Page de Menu

void MenuState::handleEvent() {
	sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();

        // Interaction avec les boutons
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (menu.PlayIsPressed(mousePos)) {
				regieSon.jouerSon(SoundEffectNames::Click, 100.f);
				machine.addState(StatesNames::Game, std::move(std::make_unique<GameState>(machine, window, regieSon)));
                machine.changeStateRequest(StatesNames::Game);
				regieSon.playMusic(MusicNames::MusicGame);
            }
            if (menu.CharacterIsPressed(mousePos)) {
                // machine.changeState(new MenuState(machine, fenetre));
            }
			if (menu.SettingsIsPressed(mousePos)) {
                // machine.changeState(new MenuState(machine, fenetre));
            }
        }
    }
}	

void MenuState::update(float dt) {
	sf::Vector2i posSouris = sf::Mouse::getPosition(window);
    menu.update(dt, posSouris);
}

void MenuState::render() {
	window.clear();
	menu.afficherTitre(window);
	window.display();
}


void BreakState::handleEvent() {
	sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();

        // Interaction avec les boutons
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            regieSon.pauseMusic();
			if (pause.PlayIsPressed(mousePos)) {
				regieSon.jouerSon(SoundEffectNames::Click, 100.f);
                machine.changeStateRequest(StatesNames::Game);
				regieSon.playMusic(MusicNames::MusicGame);
            }
            if (pause.MenuIsPressed(mousePos)) {
				regieSon.jouerSon(SoundEffectNames::Click, 100.f);
                machine.changeStateRequest(StatesNames::Menu);
				regieSon.playMusic(MusicNames::MusicMenu);
            }
			if (pause.SettingsIsPressed(mousePos)) {
                // machine.changeState(new MenuState(machine, fenetre));
            }
        }
    }
}	

void BreakState::update(float dt) {
	sf::Vector2i posSouris = sf::Mouse::getPosition(window);
    pause.update(dt, posSouris);
}

void BreakState::render() {
	window.clear();
	pause.afficherTitre(window);
	window.display();
}