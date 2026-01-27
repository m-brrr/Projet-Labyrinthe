//menu de début de jeu : il faut appuyer sur START pour commencer le jeu.
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <iostream>
#include <stack>

#include "terrain.hpp"
#include "labyrinthe.hpp"
#include "personnage.hpp"
#include "enum_types.hpp"
#include "gameFilter.hpp"
#include "RayTracing.hpp"
#include "game_over.hpp"
#include "page_menu.hpp"
#include "page_pause.hpp"
#include "sorts.hpp"

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

		playerPerso monPerso;
		
		std::vector<std::unique_ptr<Spell>> spells;
		std::vector<std::unique_ptr<monster>> allEnemies;

		sf::RenderTexture lightmap;
		sf::Sprite lightSprite;
		WhatUSee myView;
	
		
	public :

	//Constructeur :
		GameState(StateMachine& machine, sf::RenderWindow& fenetre) : 
			State(machine,fenetre), 
			monPerso(playerPerso(Direction::Up, "Child")),
			grilleLaby(H, L),
			myView(),
			theMap(grilleLaby.get_grille())
				{
				std::cout<<"generation du monde en cours..."<<std::endl;
				grilleLaby.afficher();	//on l'affiche dans la console	

				theMap.load( sf::Vector2u(150,150), grilleLaby.get_grille(),L,H);
				std::cout<<"AAAAAA"<<std::endl;
				grilleLaby.afficher();	//on l'affiche dans la console	
				lightmap.create(800,600);
				allEnemies.push_back(std::make_unique<monster>(Direction::Up, "Aguy", sf::Vector2f(200.f,200.f)));
				}

	//Méthodes : 
		void handleEvent() override;
		void update(float dt) override;	//Mise à jour des données de vie et de niveau du perso
		void render() override;
};



class EndState : public State{
	private :
		affichage_fin titreFin;
		
	public :
		EndState(StateMachine& machine, sf::RenderWindow& fenetre):  State(machine, fenetre),titreFin()
			{
			std::cout<<"generation page de fin de jeu en cours..."<<std::endl;
			}

		void handleEvent() override;
		void update(float dt) override;	//Mise à jour des données de vie et de niveau du perso
		void render() override;
};



class MenuState : public State {
	private :
		affichage_menu menu;
	public : 
		//constructeur
		MenuState(StateMachine& machine, sf::RenderWindow& fenetre) : State(machine, fenetre)
		{
			std::cout<<"generation page menu du jeu en cours..."<<std::endl;
		}
		//méthodes 

		void handleEvent() override;
		void update(float dt) override;	//Mise à jour des données de vie et de niveau du perso
		void render() override;
};


class BreakState : public State {
	private :
		affichage_pause pause;
	public : 
		//constructeur
		BreakState(StateMachine &machine, sf::RenderWindow& fenetre) : State(machine, fenetre)
		{
			std::cout<<"generation page de pause du jeu en cours..."<<std::endl;
		}
		//méthodes 

		void handleEvent() override;
		void update(float dt) override;	//Mise à jour des données de vie et de niveau du perso
		void render() override;
};

/*
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
	*/

class StateMachine {
private:
    // Utiliser une map permet d'accéder à l'état par son ID sans utiliser de chiffres (un peu comme un dictionnaire python)
    std::map<StatesNames, std::unique_ptr<State>> states;
    StatesNames currentState;
	StatesNames newState;
    bool hasCurrentState = false;
	bool changeStateFlag = false;


public:
    // Ajoute ou remplace un état (move détruit automatiquement un état)
    void addState(StatesNames id, std::unique_ptr<State> newState) {
        states[id] = std::move(newState); 
    }

    // Change l'état actif avec vérification de sécurité
    void setCurrentState(StatesNames id) {
            currentState = id;
            hasCurrentState = true;
    }

    // Accesseur rapide pour l'état actuel
    State* getCurrent() {
        return hasCurrentState ? states[currentState].get() : nullptr;
    }
    
    void handleEvent() {
        if (auto* state = getCurrent()) {
            state->handleEvent();
        }
    }

    void update(float dt) {
        if (auto* state = getCurrent()) {
            state->update(dt);
        }
    }

    void render() {
        if (auto* state = getCurrent()) {
            state->render();
        }
    }

    // Supprimer un état pour libérer de la mémoire
    void removeState(StatesNames id) {
        states.erase(id);
        if (currentState == id) hasCurrentState = false;
    }

	void changeStateRequest(StatesNames id){
		newState= id;
		changeStateFlag=true;
	}

	void changeStateApply(){
		if (changeStateFlag){
			setCurrentState(newState);
			changeStateFlag=false;
		}
	}
};