#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "boutons.hpp"


class affichage_pause {
	private :
		sf::Text Nom_Jeu;
		sf::Text subNom_Jeu;
		sf::Font font1;

		sf::Texture backGroundTexture;
		sf::Sprite backGroundSprite;

		Button btnPlay;
		Button btnMenu;
		Button btnSettings;

		float flicker=0;
	public : 
		affichage_pause() : btnPlay(290, 420, 220, 70, L"Reprendre", sf::Color(70, 70, 70), sf::Color(100, 100, 100), sf::Color(50, 50, 50)),
			btnMenu(100, 350, 180, 50, "Menu", sf::Color(70, 70, 70), sf::Color(100, 100, 100), sf::Color(50, 50, 50)),
			btnSettings(520, 350, 180, 50, L"Reglages", sf::Color(70, 70, 70), sf::Color(100, 100, 100), sf::Color(50, 50, 50))
			{

			
			if (!font1.loadFromFile("./assets/police/they-perished/TheyPerished.ttf")) {
		       throw( "Erreur : Impossible de charger la police 'TheyPerished.ttf'." );
		    }

		    if (!backGroundTexture.loadFromFile("./assets/autre/chemin-dans-la-forêt-sombre-et-effrayante.png")) throw( "Erreur : Impossible de charger l'image de fond." );;
		    
			backGroundSprite.setTexture(backGroundTexture);
			backGroundSprite.setScale(800.f / backGroundTexture.getSize().x, 600.f / backGroundTexture.getSize().y);	//redimensionner l'image à la taille de la fenêtre

			
			Nom_Jeu.setString("Pause");
			Nom_Jeu.setFont(font1);
			Nom_Jeu.setCharacterSize(80);
		    Nom_Jeu.setFillColor(sf::Color::White);
		    Nom_Jeu.setStyle(sf::Text::Bold);

		    // Centrage du titre
		    Nom_Jeu.setOrigin(Nom_Jeu.getLocalBounds().width / 2, Nom_Jeu.getLocalBounds().height / 2);
		    Nom_Jeu.setPosition(400, 150);

		    subNom_Jeu.setFont(font1);
		    subNom_Jeu.setString(L"N'ayez pas peur...");
		    subNom_Jeu.setCharacterSize(30);
		    subNom_Jeu.setFillColor(sf::Color::White);
		    subNom_Jeu.setOrigin(subNom_Jeu.getLocalBounds().width / 2.f, subNom_Jeu.getLocalBounds().height / 2.f);
		    subNom_Jeu.setPosition(400, 250);
		}

		void update(float dt, sf::Vector2i posSouris){
			flicker+=0.005;
			Nom_Jeu.setPosition(400,150+std::sin(flicker)*5);
			subNom_Jeu.setPosition(400,250+std::sin(flicker+1)*5);
			btnPlay.update(posSouris);
    		btnMenu.update(posSouris);
			btnSettings.update(posSouris);
		}

		void afficherTitre(sf::RenderWindow& window) {
			window.draw(backGroundSprite);
			window.draw(Nom_Jeu);
			window.draw(subNom_Jeu);
			btnMenu.afficherBouton(window);
			btnPlay.afficherBouton(window);
			btnSettings.afficherBouton(window);
		}

		bool PlayIsPressed(sf::Vector2i mousePos){
			return btnPlay.isPressed(mousePos);
		}

		bool SettingsIsPressed(sf::Vector2i mousePos) {
			return btnSettings.isPressed(mousePos);
		}

		bool MenuIsPressed(sf::Vector2i mousePos) {
			return btnMenu.isPressed(mousePos);
		}
};

