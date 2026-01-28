#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "boutons.hpp"


class AffichageSettings {
private:
    sf::Font font;
    sf::Sprite background;
    sf::Texture backGroundTexture;

    // Volume SFX
    sf::Text volumeSFXTitle;
    sf::RectangleShape volumeSFXBar;
    sf::RectangleShape volumeSFXCursor;

	// Volume Musique
    sf::Text volumeMusicTitle;
    sf::RectangleShape volumeMusicBar;
    sf::RectangleShape volumeMusicCursor;

    // Difficulté
    sf::Text diffTitle;
    std::vector<sf::Text> diffOptions;

    // Bouton retour
    Button backButton;

public:
    AffichageSettings() : backButton(300, 500, 200, 70, L"RETOUR", sf::Color(70, 70, 70), sf::Color(100, 100, 100), sf::Color(50, 50, 50)) {
        if (!font.loadFromFile("./assets/police/endor/ENDORALT.ttf")) {
		       throw( "Erreur : Impossible de charger la police 'endor'." );
		    }

		if (!backGroundTexture.loadFromFile("./assets/autre/chemin-dans-la-forêt-sombre-et-effrayante.png")) {
			throw( "Erreur : Impossible de charger l'image de fond." );
		}

        background.setTexture(backGroundTexture);
		background.setScale(800.f / backGroundTexture.getSize().x, 600.f / backGroundTexture.getSize().y);

        // Configuration Volume SFX
        volumeSFXTitle.setFont(font);
        volumeSFXTitle.setString("Volume SFX");
        volumeSFXTitle.setPosition(150, 100);

        volumeSFXBar.setSize(sf::Vector2f(300, 20));
        volumeSFXBar.setFillColor(sf::Color(100, 100, 100));
        volumeSFXBar.setPosition(300, 100);

        volumeSFXCursor.setSize(sf::Vector2f(20, 25));
        volumeSFXCursor.setOrigin(10.f, 12.5f);
        volumeSFXCursor.setFillColor(sf::Color::Red);

		 // Configuration Volume Musique
        volumeMusicTitle.setFont(font);
        volumeMusicTitle.setString("Volume Musique");
        volumeMusicTitle.setPosition(150, 250);

        volumeMusicBar.setSize(sf::Vector2f(300, 20));
        volumeMusicBar.setFillColor(sf::Color(100, 100, 100));
        volumeMusicBar.setPosition(300, 250);

        volumeMusicCursor.setSize(sf::Vector2f(20, 25));
        volumeMusicCursor.setOrigin(10.f, 12.5f);
        volumeMusicCursor.setFillColor(sf::Color::Red);

        // Configuration Difficulté

		diffTitle.setFont(font);
        diffTitle.setString("Difficulte");
        diffTitle.setPosition(150, 400);

        std::vector<std::string> labels = {"Facile", "Normal", "Difficile"};
        for (int i = 0; i < 3; ++i) {
            sf::Text t;
            t.setFont(font);
            t.setString(labels[i]);
            t.setCharacterSize(30);
            t.setPosition(350 + (i * 100), 400);
            diffOptions.push_back(t);
        }
    }

    void updateVolumeSFXVisual(float volume) {
        // volume est entre 0 et 100, on le replace sur la barre (300px de large)
        volumeSFXCursor.setPosition(300 + (volume * 3.f), 110);
    }

	void updateVolumeMusicVisual(float volume) {
        // volume est entre 0 et 100, on le replace sur la barre (300px de large)
        volumeMusicCursor.setPosition(300 + (volume * 3.f), 260);
    }

    void updateDifficultyVisual(int currentDiff) {
        for (int i = 0; i < 3; ++i) {
            diffOptions[i].setFillColor(i == currentDiff ? sf::Color::Red : sf::Color::White);
            diffOptions[i].setScale(i == currentDiff ? sf::Vector2f(1.1f, 1.1f) : sf::Vector2f(1.f, 1.f));
        }
    }

    void dessiner(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(volumeSFXTitle);
        window.draw(volumeSFXBar);
        window.draw(volumeSFXCursor);
		window.draw(volumeMusicTitle);
        window.draw(volumeMusicBar);
        window.draw(volumeMusicCursor);
		window.draw(diffTitle);
        for (auto& t : diffOptions) window.draw(t);
        backButton.afficherBouton(window);
    }

    // Getters pour les collisions (clics souris)
    sf::FloatRect getSFXVolumeBounds() { return volumeSFXBar.getGlobalBounds(); }
	sf::FloatRect getMusicVolumeBounds() { return volumeMusicBar.getGlobalBounds(); }
    sf::FloatRect getDiffBounds(int i) { return diffOptions[i].getGlobalBounds(); }
    sf::FloatRect getBackBounds() { return backButton.getGlobalBounds(); }
};