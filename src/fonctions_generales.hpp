#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "enum_types.hpp"


sf::Vector2f getRayImpact(sf::Vector2f playerPos, float angle, const std::vector<std::vector<int>>& maze, int tileWidth);