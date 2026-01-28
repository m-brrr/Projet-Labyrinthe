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
								spells.push_back(std::make_unique<Spell>(400,300, Level, maDirection, typeDeSpell, monPerso, theMap.getPosition()));	//On rajoute le sort à la liste des choses à afficher
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
							theMap.moveTheMap(dt, Direction::Left, 150.f);
							LabyMov=theMap.getMov(dt, Direction::Left);

							monPerso.setDirection(Direction::Left);
							monPerso.perso_animateMov(regieSon);
						}
				        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			            {
							theMap.moveTheMap( dt, Direction::Right, 150.f);
							LabyMov=theMap.getMov(dt, Direction::Right);

							monPerso.setDirection(Direction::Right);
							monPerso.perso_animateMov(regieSon);
						}
				        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			            {
							theMap.moveTheMap( dt, Direction::Up, 150.f);
							LabyMov=theMap.getMov(dt, Direction::Up);

							monPerso.setDirection(Direction::Up);
							monPerso.perso_animateMov(regieSon);
						}
				        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			            {
							theMap.moveTheMap (dt, Direction::Down, 150.f);
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

					//On vérifie si le joueur a gagné 
						if(theMap.didWin(H,L,150.f)) {
							machine.addState(StatesNames::HappyEnd, std::move(std::make_unique<WinState>(machine, window, regieSon)));
                			machine.changeStateRequest(StatesNames::HappyEnd);
						}
						
					//Mise à jour des enemis
						for (auto it = allEnemies.begin(); it != allEnemies.end(); ) {
							(*it)->update(playerPos,grilleLaby.get_grille(),150.f,dt, spells, theMap, regieSon);
							
							++it; // Passe au monstre suivant
						}

					//Mise à jour des sorts
						for (auto it = spells.begin(); it != spells.end(); ) {
							bool spellDestroyed = false;
							//on met à jour la position du spell, sauf s'il rencontre un mur.
							sf::Sprite* theSpellSprite=(*it)->getSprite();
							//bool toucheMur=theMap.canMove(it,  )
							if(!(*it)->setPosition(dt, theMap, 150.f)) spellDestroyed=true;
							(*it)->Spell_animateMov();
							

							//on vérifie le spell touche le personnage
						    if ((*it)->didTouchCharacter(monPerso.getBoundsCharacter(),monPerso)) {
						        std::cout << "YESSS" << std::endl;
						        monPerso.take_damage((*it)->getSpellLevel());
						        spellDestroyed = true; // Supprime l'élément et met à jour l'itérateur
						    } 

							//on vérifie si le spell touche les enemis
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

							//si le spell est détruit, on l'efface de la mémoire
							if (spellDestroyed) it = spells.erase(it);
							else {it++;}
						}
						
						//On vérifie si les enemis sont morts :
						for (auto enemyIt = allEnemies.begin(); enemyIt != allEnemies.end(); ) {
						    if (!(*enemyIt)->isAlive()) {
						        std::cout << "Un ennemi vient de mourir !" << std::endl;
						        enemyIt = allEnemies.erase(enemyIt);
						    } else {
						        ++enemyIt;
						    }
						}
						
						//on verifie si le perso est mort :
						if (monPerso.getHealthPoints()<=0){
							std::cout<<"You Lost"<<std::endl;
							machine.addState(StatesNames::GameOver, std::move(std::make_unique<LooseState>(machine, window, regieSon)));
                			machine.changeStateRequest(StatesNames::GameOver);
						}

						//on update le rayCasting avant affichage
						sf::Vector2f exitPosWorld((L - 1) * 150.f, (H - 1) * 150.f);
						myView.update(grilleLaby.get_grille(), playerPos, 150.f, exitPosWorld,theMap.getExitGlowClock());
						myView.setPosition(mapPos);	
}

void GameState::render() {		//méthode pour tout afficher

			lightmap.clear(sf::Color(10,10,20));//quasi noir
			lightmap.draw(myView,sf::BlendAdd);
			lightmap.display();

			window.clear();
			
			window.draw(theMap);
			theMap.afficher_porte(window, L, H);

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

void GameState::generer_enemis(float tileWidth) {
    std::vector<sf::Vector2i> emptyTiles;
	std::vector<std::vector<int>> maze=grilleLaby.get_grille();
    //Lister toutes les cases avec des 0 (libres)
    for (int j = 0; j < maze.size(); ++j) {
        for (int i = 0; i < maze[j].size(); ++i) {
            if (maze[j][i] == 0) { 
                emptyTiles.push_back(sf::Vector2i(i, j));
            }
        }
    }

    // 2. Générer les ennemis
    std::random_device rd;
    std::mt19937 gen(rd());
    
    for (int i = 0; i < nbEnemis && !emptyTiles.empty(); ++i) {
        std::uniform_int_distribution<> dis(0, emptyTiles.size() - 1);
        int index = dis(gen);
        sf::Vector2i tile = emptyTiles[index];

        // Calcul de la position absolue (centrée dans la tuile)
        float posX = tile.x * tileWidth + tileWidth / 2.f;
        float posY = tile.y * tileWidth + tileWidth / 2.f;

        // on cree l'ennemi 
         allEnemies.push_back(std::make_unique<monster>(Direction::Up, "Aguy", sf::Vector2f(posX,posY)));

        // on retire la case pour ne pas spawner deux ennemis au même endroit
        emptyTiles.erase(emptyTiles.begin() + index);
    }
}
		//Page de Fin de Jeu

void LooseState::handleEvent() {
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

void LooseState::update(float dt) {
	sf::Vector2i posSouris = sf::Mouse::getPosition(window);
    titreFin.update(dt, posSouris);
}

void LooseState::render() {
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
                regieSon.jouerSon(SoundEffectNames::Click, 100.f);
				machine.addState(StatesNames::Settings, std::move(std::make_unique<SettingsState>(machine, window, regieSon)));
                machine.changeStateRequest(StatesNames::Settings);
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

		//Page de mise en pause

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

//Page Victoire :

void WinState::handleEvent() {
	sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();

        // Interaction avec les boutons
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (victoire.RetryIsPressed(mousePos)) {
				regieSon.jouerSon(SoundEffectNames::Click, 100.f);
				machine.addState(StatesNames::Game, std::move(std::make_unique<GameState>(machine, window, regieSon)));
                machine.changeStateRequest(StatesNames::Game);
            }
            if (victoire.MenuIsPressed(mousePos)) {
				regieSon.jouerSon(SoundEffectNames::Click, 100.f);
                machine.addState(StatesNames::Menu, std::move(std::make_unique<MenuState>(machine, window, regieSon)));
                machine.changeStateRequest(StatesNames::Menu);
            }
        }
    }
}	

void WinState::update(float dt) {
	sf::Vector2i posSouris = sf::Mouse::getPosition(window);
    victoire.update(dt, posSouris);
}

void WinState::render() {
	window.clear();
	victoire.afficherTitre(window);
	window.display();
}

//Page settings :

void SettingsState::handleEvent()  {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Logique Volume
                if (affichagereglages.getSFXVolumeBounds().contains(mPos)) {
                    float nouveauVolume = (mPos.x - 300) / 3.f;
                    regieSon.setSFXVolume(nouveauVolume);
                    affichagereglages.updateVolumeSFXVisual(nouveauVolume);
                }

				if (affichagereglages.getMusicVolumeBounds().contains(mPos)) {
                    float nouveauVolume = (mPos.x - 300) / 3.f;
                    regieSon.setMusicVolume(nouveauVolume);
                    affichagereglages.updateVolumeMusicVisual(nouveauVolume);
                }

                // Logique Difficulté
                for (int i = 0; i < 3; ++i) {
                    if (affichagereglages.getDiffBounds(i).contains(mPos)) {
                        difficultéInterne = i;
                        affichagereglages.updateDifficultyVisual(i);
                        regieSon.jouerSon(SoundEffectNames::Click, 100.f);
                    }
                }

                // Logique Retour
                if (affichagereglages.getBackBounds().contains(mPos)) {
                    machine.changeStateRequest(StatesNames::Menu);
                }
            }
        }
    }

void SettingsState::render() {
        window.clear();
        affichagereglages.dessiner(window);
        window.display();
    }