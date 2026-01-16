#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

int main() {
    // Créer une fenêtre
    sf::RenderWindow window(sf::VideoMode(800, 600), "Jeu SFML");

    // Créer un carré (le joueur)
    sf::RectangleShape player(sf::Vector2f(50.f, 50.f));
    player.setFillColor(sf::Color::Green);
    player.setPosition(400.f, 300.f);

	//créée des carrés (obstacles)

	sf::RectangleShape obstacle_1(sf::Vector2f(100.f, 100.f));
    obstacle_1.setFillColor(sf::Color::Red);
    obstacle_1.setPosition(300.f, 400.f);

	sf::RectangleShape obstacle_2(sf::Vector2f(100.f, 100.f));
    obstacle_2.setFillColor(sf::Color::Red);
    obstacle_2.setPosition(110.f, 130.f);

	sf::Font font1;
	font1.loadFromFile("../assets/they-perished/TheyPerished.ttf");
	sf::Text Nom_Jeu ("Labyrinthe", font1);
	Nom_Jeu.setCharacterSize(32);


    // Horloge pour le mouvement fluide
    sf::Clock clock;

    while (window.isOpen()) {
        // Gestion des événements
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Déplacement du joueur avec le clavier
        float speed = 200.f; // Vitesse en pixels par seconde
        float deltaTime = clock.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {obstacle_1.move(speed * deltaTime, 0.f);
			obstacle_2.move(speed * deltaTime, 0.f);}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {obstacle_1.move(-speed * deltaTime, 0.f);
			obstacle_2.move(-speed * deltaTime, 0.f);}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {obstacle_1.move(0.f, speed * deltaTime);
			obstacle_2.move(0.f, speed * deltaTime);}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {obstacle_1.move(0.f, -speed * deltaTime);
			obstacle_2.move(0.f, -speed * deltaTime);}

        // Effacer la fenêtre
        window.clear();

        // Dessiner le joueur
        window.draw(player);
		window.draw(obstacle_1);
		window.draw(obstacle_2);
		window.draw(Nom_Jeu);

        // Afficher la fenêtre
        window.display();
    }

    return 0;
}