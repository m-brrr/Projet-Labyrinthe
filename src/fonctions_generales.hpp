#pragma once
#include "enum_types.hpp"
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "sorts.hpp"
#include "personnage.hpp"

void verifierCollisions() {
        for (auto& spell : spells) {
            for (auto& personnage : personnages) {
                if (spell.touchePersonnage(personnage.getGlobalBounds())) {
                    // Gérer la collision (ex: infliger des dégâts)
                }
            }
        }
    }