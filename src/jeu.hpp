#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>


int jeu() {
// Créer une fenêtre
    sf::RenderWindow window(sf::VideoMode(800, 600), "Jeu SFML");


//Créer des carrés (obstacles)
	sf::RectangleShape obstacle_1(sf::Vector2f(100.f, 100.f));
    obstacle_1.setFillColor(sf::Color::Red);
    obstacle_1.setPosition(800.f, 900.f);

	sf::RectangleShape obstacle_2(sf::Vector2f(100.f, 100.f));
    obstacle_2.setFillColor(sf::Color::Red);
    obstacle_2.setPosition(110.f, 130.f);

//Créer le titre
	sf::Font font1;
	if (!font1.loadFromFile("./assets/police/they-perished/TheyPerished.ttf")) {
        std::cerr << "Erreur : Impossible de charger la police 'TheyPerished.ttf'." << std::endl;
        return -1; // Quitter si la police n'est pas chargée
    }

	sf::Text Nom_Jeu ("Labyrinthe", font1);
	Nom_Jeu.setCharacterSize(32);
	Nom_Jeu.setFillColor(sf::Color::White);
	Nom_Jeu.setPosition(110.f, 130.f);

//Créer le personnage : voir fichier personnage

// Horloge pour le mouvement et l'animation
    sf::Clock speed_clock;
	sf::Clock animation_clock;
	float animation_time= animation_clock.getElapsedTime().asSeconds()

    while (window.isOpen()) {
        // Gestion des événements
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
	//Pour le déplacement
        float speed = 200.f; 
        float deltaTime = speed_clock.restart().asSeconds();
		
	//Pour l'animation	
		float animationDelay=0.1f;
		static int direction=0;
		static int posture=0;
		//Condition est ce qu'une flêche a été appuyée :
	
		if (isAnyArrowKeyPressed()) {

	        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	            {
					direction=3;
					obstacle_1.move(speed * deltaTime, 0.f);
					obstacle_2.move(speed * deltaTime, 0.f);
				}
	        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	            {
					direction=2;
					obstacle_1.move(-speed * deltaTime, 0.f);
					obstacle_2.move(-speed * deltaTime, 0.f);
			}
	        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	            {
					direction=1;
					obstacle_1.move(0.f, speed * deltaTime);
					obstacle_2.move(0.f, speed * deltaTime);
			}
	        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	            {
					direction=0;
					obstacle_1.move(0.f, -speed * deltaTime);
					obstacle_2.move(0.f, -speed * deltaTime);
			}
			if (animation_clock.getElapsedTime().asSeconds()>= animationDelay){
				posture= (posture+1)%8;
				animation_clock.restart();
			}
		}
		switch (direction) {
            case 0: character.setTextureRect(rectFace[posture]); break;
            case 1: character.setTextureRect(rectBack[posture]); break;
            case 2: character.setTextureRect(rectRight[posture]); break;
            case 3: character.setTextureRect(rectLeft[posture]); break;
        }

        // Effacer la fenêtre
        window.clear();

        // Dessiner le joueur
        window.draw(character);
		window.draw(obstacle_1);
		window.draw(obstacle_2);
		window.draw(Nom_Jeu);
        // Afficher la fenêtre
        window.display();
			}

    return 0;
}