//menu de début de jeu : il faut appuyer sur START pour commencer le jeu.
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "terrain.hpp"
#include "labyrinthe.hpp"
#include "personnage.hpp"
#include "enum_types.hpp"
#include "gameFilter.hpp"
#include "RayTracing.hpp"
#include <iostream>

class StateMachine; 	//On la déclare juste pour pouvoir l'utiliser après

class State {
	protected : 
		StateMachine& machine;
		sf::RenderWindow& window;
	
	public: 
		//constructeur
		State(StateMachine& LaMachine, sf::RenderWindow& fenetre) :machine(LaMachine), window(fenetre){};
		
		//destructeur
		virtual ~State() = default;

		//méthodes à faire dans l'ordre
		virtual void handleEvent()=0;	//gère les évenements
		virtual void update(float dt)=0;	//Met à jour l'Etat
		virtual void render()=0;	//Affiche l'état à l'écran (la fenêtre est membre donc pas besoin de la redefinir)
};


class GameState : public State {
	private :
		
		const int H = 19; //dimensions du Labyrinthe
    	const int L = 31;

    	grilleLabyrinthe grilleLaby; //La grille du labyrinthe (constituée de 0 et de 1)
		Map theMap;		//Le labyrinthe réel (généré à partir de la grille)

		BarreDeVie maBarre;
		personnage monPerso;
		
		std::vector<std::unique_ptr<Spell>> spells;
		std::vector<std::unique_ptr<personnage>> allEnemies;

		sf::RenderTexture lightmap;
		sf::Sprite lightSprite;
		WhatUSee myView;
	
		
	public :

	//Constructeur :
		GameState(StateMachine& machine, sf::RenderWindow& fenetre) : 
			State(machine,fenetre), 
			maBarre(),
			monPerso(personnage(1.f, Direction::Up, "Child", maBarre)),
			grilleLaby(H, L),
			myView()
				{
				std::cout<<"generation du monde en cours..."<<std::endl;
				grilleLaby.afficher();	//on l'affiche dans la console	

				theMap.load( sf::Vector2u(150,150), grilleLaby.get_grille(),L,H);
				std::cout<<"AAAAAA"<<std::endl;
				grilleLaby.afficher();	//on l'affiche dans la console	
				lightmap.create(800,600);
				}

	//Méthodes : 
		void handleEvent() override;
		void update(float dt) override;	//Mise à jour des données de vie et de niveau du perso
		void render() override;
};

/*
class MenuState : public State {
	public : 
		//constructeur
		MenuState(StateMachine& machine, sf::RenderWindow& fenetre) : State(machine, fenetre){}

		//destructeur
		~MenuState() override {};

		//méthodes 

		void()
};

class SettingsState : public State{
	public :
};

class BreakState : public State {
	public :
};

class ChooseCharacterState : public State {
	public : 
};

void affichage_menu(sf::RenderWindow window) {
	// Effacer la fenêtre
        window.clear();
	sf::Font font1;
	font1.loadFromFile("../assets/they-perished/TheyPerished.ttf");
	sf::Text Nom_Jeu ("Labyrinthe", font1);
	Nom_Jeu.setCharacterSize(32);
	window.draw(Nom_Jeu);
}

class bouton {
	private :
		int x;
		int y;
		std::unique_ptr<sf::Shape> forme;
		std::unique_ptr<sf::Color> couleur;
		sf::Text text;	//potentiellement vide
	public :
		bouton(int posX,int posY, std::unique_ptr<sf::Shape> shape, std::unique_ptr<sf::Color> color) : x(posX), y(posY), forme(std::move(shape)), couleur(std::move(color)){}

		int getX(){
			return x;
		}
		int getY(){
			return y;
		}
		sf::Shape& getShape() {
        	return *forme;
    	}
		void text(std::string string){

		}
}

*/

class StateMachine {	//gestionnaire des différents états
	private:
		std::unique_ptr<State> current;
	public : 
	void setState(std::unique_ptr<State> newState){
		current = std::move(newState);
	}
	void handleEvent() {current->handleEvent(); }
	void update(float dt) {current->update(dt);}
	void render() {current->render();}

};