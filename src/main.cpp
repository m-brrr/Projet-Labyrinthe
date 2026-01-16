#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "etats_jeu.hpp"
#include "terrain.hpp"
#include "personnage.hpp"


int main() {
	sf::RenderWindow window(sf::VideoMode(800,600), "Labyrinthe");
    StateMachine machine;
    machine.setState(std::make_unique<GameState>(machine, window));

	sf::Clock gameClock;

    while (window.isOpen()) {
		float dt = gameClock.restart().asSeconds(); //à chaque frame
        machine.handleEvent();
        machine.update(dt);
        machine.render();
    }
}