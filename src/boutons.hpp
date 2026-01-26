#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
	sf::Font font;
    sf::Color PassiveColor;
    sf::Color hoverColor;
    sf::Color activeColor;

public:
    Button(float x, float y, float width, float height, sf::String message, 
           sf::Color passive, sf::Color hover, sf::Color active) {

				//Chargement police
				
				if (!font.loadFromFile("./assets/police/they-perished/TheyPerished.ttf")) {
				       throw( "Erreur : Impossible de charger la police 'TheyPerished.ttf'." );
				    }
		        // Configuration du rectangle
		        shape.setPosition(sf::Vector2f(x, y));
		        shape.setSize(sf::Vector2f(width, height));
		        
		        PassiveColor = passive;
		        hoverColor = hover;
		        activeColor = active;
		        shape.setFillColor(PassiveColor);

		        // Configuration du texte
		        text.setFont(font);
		        text.setString(message);
		        text.setCharacterSize(24);
		        text.setFillColor(sf::Color::White);

		        // Centrage automatique du texte dans le bouton
		        sf::FloatRect textBounds = text.getLocalBounds();
		        text.setOrigin(textBounds.left + textBounds.width / 2.0f, 
		                       textBounds.top + textBounds.height / 2.0f);	//on définit l'origine du texte au centre
		        text.setPosition(x + width / 2.0f, y + height / 2.0f);	//et on le positionne au centre du bouton
		    }

    // Vérifie si la souris survole le bouton et change sa couleur
    void update(const sf::Vector2i& mousePos) {
        if (shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            shape.setFillColor(hoverColor);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                shape.setFillColor(activeColor);
            }
        } else {
            shape.setFillColor(PassiveColor);
        }
    }

    // Vérifie si le bouton est cliqué
    bool isPressed(const sf::Vector2i& mousePos) {
        return shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) 
               && sf::Mouse::isButtonPressed(sf::Mouse::Left);
    }

    void afficherBouton(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }
};