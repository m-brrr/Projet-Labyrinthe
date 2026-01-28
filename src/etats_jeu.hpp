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
#include "victoire.hpp"
#include "page_pause.hpp"
#include "sorts.hpp"
#include "gestion_son.hpp"
#include "fonctions_generales.hpp"
#include "Settings.hpp"
#include "page_personnage.hpp"

class StateMachine; 	//On la déclare juste pour pouvoir l'utiliser après

class State {
	protected : 
		StateMachine& machine;
		sf::RenderWindow& window;
		Son& regieSon;
	
	public: 
		//constructeur
		State(StateMachine& LaMachine, sf::RenderWindow& fenetre, Son& leSon) :machine(LaMachine), window(fenetre), regieSon(leSon){};
		
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

    	
		Map theMap;		//Le labyrinthe réel (généré à partir de la grille)
		grilleLabyrinthe grilleLaby; //La grille du labyrinthe (constituée de 0 et de 1)
		std::string persoChoisi;
		playerPerso monPerso;
		
		std::vector<std::unique_ptr<Spell>> spells;
		std::vector<std::unique_ptr<monster>> allEnemies;
		int nbEnemis=5;
		sf::RenderTexture lightmap;
		sf::Sprite lightSprite;
		WhatUSee myView;

		
	public :

	//Constructeur :
		GameState(StateMachine& machine, sf::RenderWindow& fenetre, Son& leSon);

	//Méthodes : 
		void generer_enemis(float tileWidth);
		void handleEvent() override;
		void update(float dt) override;	//Mise à jour des données de vie et de niveau du perso
		void render() override;
};

class LooseState : public State{
	private :
		affichage_fin titreFin;
		
	public :
		LooseState(StateMachine& machine, sf::RenderWindow& fenetre, Son& leSon):  State(machine, fenetre, leSon),titreFin()
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
		MenuState(StateMachine& machine, sf::RenderWindow& fenetre, Son& leSon) : State(machine, fenetre, leSon), menu()
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
		BreakState(StateMachine &machine, sf::RenderWindow& fenetre, Son& leSon) : State(machine, fenetre, leSon)
		{
			std::cout<<"generation page de pause du jeu en cours..."<<std::endl;
		}
		//méthodes 

		void handleEvent() override;
		void update(float dt) override;	//Mise à jour des données de vie et de niveau du perso
		void render() override;
};

class WinState : public State{
	private :
		affichage_victoire victoire;
		
	public :
		WinState(StateMachine& machine, sf::RenderWindow& fenetre, Son& leSon):  State(machine, fenetre, leSon),victoire()
			{
			std::cout<<"generation page de victoire en cours..."<<std::endl;
			}

		void handleEvent() override;
		void update(float dt) override;	//Mise à jour des données de vie et de niveau du perso
		void render() override;
};

class SettingsState : public State {
private:
    AffichageSettings affichagereglages;
    int difficultéInterne = 1;

public:
    SettingsState(StateMachine& machine, sf::RenderWindow& window, Son& leSon);

    void handleEvent() override;

    void update(float dt) override {
        // Ici, on pourrait ajouter des animations fluides si besoin
    }

    void render() override;
};

class CharacterState : public State {
private:
    AffichageCharacter Affichagepersonnage;
    int choixInitial = 0;

public:
    CharacterState(StateMachine& machine, sf::RenderWindow& window, Son& leSon);

    void handleEvent() override;

    void update(float dt) override {
        // Ici, on pourrait ajouter des animations fluides si besoin
    }

    void render() override;
};

