#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "etats_jeu.hpp"
#include "terrain.hpp"
#include "personnage.hpp"
#include "enum_types.hpp"
#include "gestion_son.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode(800,600), "Labyrinthe");
    StateMachine machine;
	Son regieSon;
    machine.addState(StatesNames::HappyEnd, std::make_unique<WinState>(machine, window,regieSon));
	machine.setCurrentState(StatesNames::HappyEnd);
	regieSon.playMusic(MusicNames::MusicMenu);

	sf::Clock gameClock;

    while (window.isOpen()) {
		float dt = gameClock.restart().asSeconds(); //à chaque frame
        machine.handleEvent();
        machine.update(dt);

		window.clear();
        machine.render();
		window.display();

		machine.changeStateApply();
    }
}