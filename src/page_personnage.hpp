#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "boutons.hpp"


class AffichageCharacter {
private:
	std::vector<std::string> names = {"Child", "CowBoy", "Princess", "Warrior"};
    std::vector<sf::Texture> charactersTextures;
    std::vector<sf::Sprite> charactersSprites;
    std::vector<sf::Text> nameOptions; // Anciennement diffOptions
    
    sf::Font font;
    const int spriteWidth = 77;
    const int spriteHeight = 77;
    // Difficulté
    sf::Text Title;

	sf::Texture backGroundTexture;
    sf::Sprite background;

    // Bouton retour
    Button backButton;

public:
    AffichageCharacter() : backButton(300, 500, 200, 70, L"RETOUR", sf::Color(70, 70, 70), sf::Color(100, 100, 100), sf::Color(50, 50, 50)) {
        
		if (!font.loadFromFile("./assets/police/endor/ENDORALT.ttf")) {
		       throw( "Erreur : Impossible de charger la police 'endor'." );
		    }

		if (!backGroundTexture.loadFromFile("./assets/autre/chemin-dans-la-forêt-sombre-et-effrayante.png")) {
			throw( "Erreur : Impossible de charger l'image de fond." );
		}
		
		background.setTexture(backGroundTexture);
		background.setScale(800.f / backGroundTexture.getSize().x, 600.f / backGroundTexture.getSize().y);

		
		Title.setFont(font);
        Title.setString("Choix Personnage");
        Title.setPosition(280, 20);
		Title.setCharacterSize(70);


		//La taille des personnages est de 77
				const int spriteWidth=77;
				const int spriteHeight=77;

		charactersTextures.resize(names.size());
		charactersSprites.resize(names.size());

		for (size_t i = 0; i < names.size(); ++i) {
		    std::string path = "./assets/texturePersonnage/" + names[i] + ".png";
		    if (!charactersTextures[i].loadFromFile(path)) {
		        throw std::runtime_error("Échec du chargement de l'asset : " + path);
		    }

		    charactersSprites[i].setTexture(charactersTextures[i]);
		    charactersSprites[i].setTextureRect(sf::IntRect(0, 0, spriteWidth, spriteHeight));
		    charactersSprites[i].setScale(3.f, 3.f);
			charactersSprites[i].setOrigin(charactersSprites[i].getLocalBounds().width / 2.f, charactersSprites[i].getLocalBounds().height / 2.f);
		    charactersSprites[i].setPosition(170.f + (i * 150.f), 250.f);

		    sf::Text t;
		    t.setFont(font);
		    t.setString(names[i]);
		    t.setCharacterSize(30);
		    t.setPosition(150.f + (i * 150.f), 400.f);
		    nameOptions.push_back(t);
		}
    }

	std::string getCharacterChosen(int i){
		return names[i];
	}
	
    void updateChoiceVisual(int currentChoice) {
        for (int i = 0; i < 3; ++i) {
            nameOptions[i].setFillColor(i == currentChoice ? sf::Color::Red : sf::Color::White);
            nameOptions[i].setScale(i == currentChoice ? sf::Vector2f(1.1f, 1.1f) : sf::Vector2f(1.f, 1.f));
        }
    }

    void dessiner(sf::RenderWindow& window) {
	    window.draw(background);
	    window.draw(Title);

	    for (const auto& sprite : charactersSprites) {
	        window.draw(sprite);
	    }
	    for (auto& t : nameOptions) {
	        window.draw(t);
	    }
	    backButton.afficherBouton(window);
	}

    // Getters pour les collisions (clics souris)
    
    sf::FloatRect getDiffBounds(int i) { return nameOptions[i].getGlobalBounds(); }
    sf::FloatRect getBackBounds() { return backButton.getGlobalBounds(); }
};