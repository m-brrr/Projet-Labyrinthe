#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "enum_types.hpp"
#include "fonctions_generales.hpp"
#include "labyrinthe.hpp"
#include "terrain.hpp"


class WhatUSee : public sf::Drawable, public sf::Transformable {
private: 
    sf::VertexArray m_vertices;
    sf::VertexArray m_exitLightVertices;
    
    // Stocker les points du raycasting de sortie (calculés une seule fois)
    std::vector<sf::Vector2f> m_exitLightPointsCached;
    bool m_exitLightInitialized = false;
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(m_exitLightVertices, states);
        target.draw(m_vertices, states);
    }

public:
    WhatUSee() {
        m_vertices.setPrimitiveType(sf::TriangleFan);
        m_exitLightVertices.setPrimitiveType(sf::TriangleFan);
    }
    
	//Partie "LUMIERE SORTIE" -> inefficace
    // Initialiser la lumière de sortie 
    void initializeExitLight(const std::vector<std::vector<int>>& maze, 
                             sf::Vector2f exitPos, int tileWidth) {
        if (m_exitLightInitialized) return;  // Déjà fait
        
        // Calculer le raycasting
        std::vector<sf::Vector2f> exitLightPoints = exitRayCasting(maze, exitPos, tileWidth);
        m_exitLightPointsCached = sortPoint(exitLightPoints, exitPos);
        
        m_exitLightInitialized = true;
    }
    
    void update(const std::vector<std::vector<int>>& maze, sf::Vector2f playerPos, 
                int tileWidth, sf::Vector2f exitPos, sf::Clock& glowClock) {
        
        // Vision joueur
        m_vertices.clear();
        std::vector<sf::Vector2f> sortedPoints = sortPoint(rayCasting(maze, playerPos, tileWidth), playerPos);
        
        if (!sortedPoints.empty()) {
            float flicker = (rand() % 10) / 100.f;
            sf::Vertex centerVertex;
            centerVertex.position = playerPos;
            sf::Uint8 alpha = static_cast<sf::Uint8>(255.f * (0.9f + flicker));
            centerVertex.color = sf::Color(255, 220, 100, alpha);
            m_vertices.append(centerVertex);
            
            for (const auto& point : sortedPoints) {
                sf::Vertex newVertex;
                newVertex.position = point;
                newVertex.color = sf::Color(255, 220, 100, 0);
                m_vertices.append(newVertex);
            }
            
            sf::Vertex closingVertex;
            closingVertex.position = sortedPoints[0];
            closingVertex.color = sf::Color(255, 220, 100, 0);
            m_vertices.append(closingVertex);
        }
        
        // lumière de sortie
        updateExitLightAnimation(exitPos, glowClock);
    }
    
    // Seulement mettre à jour l'animation 
    void updateExitLightAnimation(sf::Vector2f exitPos, sf::Clock& glowClock) {
        if (!m_exitLightInitialized || m_exitLightPointsCached.empty()) return;
        
        m_exitLightVertices.clear();
        
        // Animation pulsante
        float pulse = 0.8f + 0.2f * std::sin(glowClock.getElapsedTime().asSeconds() * 2.0f);
        
        // Centre
        sf::Vertex centerVertex;
        centerVertex.position = exitPos;
        sf::Uint8 alpha = static_cast<sf::Uint8>(200 * pulse);
        centerVertex.color = sf::Color(255, 250, 230, alpha);
        m_exitLightVertices.append(centerVertex);
        
        // Utiliser les points déjà calculés
        for (const auto& point : m_exitLightPointsCached) {
            sf::Vertex newVertex;
            newVertex.position = point;
            newVertex.color = sf::Color(255, 250, 230, 0);
            m_exitLightVertices.append(newVertex);
        }
        
        // Fermer
        sf::Vertex closingVertex;
        closingVertex.position = m_exitLightPointsCached[0];
        closingVertex.color = sf::Color(255, 250, 230, 0);
        m_exitLightVertices.append(closingVertex);
    }
    
    // Raycasting depuis la sortie (appelée seule fois à l'initialisation)
    std::vector<sf::Vector2f> exitRayCasting(const std::vector<std::vector<int>>& maze, 
                                             sf::Vector2f exitPos, int tileWidth) {
        std::vector<sf::Vector2f> proximateCorner = listProximateCorner(maze, exitPos, tileWidth);
        std::vector<sf::Vector2f> lightPoints;
        float smallAngle = 0.0005f;
        
        for (const auto& element : proximateCorner) {
            float angle = atan2(element.y - exitPos.y, element.x - exitPos.x);
            lightPoints.push_back(getRayImpact(exitPos, angle, maze, tileWidth));
            
            angle -= smallAngle;
            lightPoints.push_back(getRayImpact(exitPos, angle, maze, tileWidth));
            
            angle += 2 * smallAngle;
            lightPoints.push_back(getRayImpact(exitPos, angle, maze, tileWidth));
        }
        return lightPoints;
    }
    
    std::vector<sf::Vector2f> rayCasting(const std::vector<std::vector<int>>& maze, 
                                         sf::Vector2f playerPos, int tileWidth) {
        std::vector<sf::Vector2f> proximateCorner = listProximateCorner(maze, playerPos, tileWidth);
        std::vector<sf::Vector2f> lightPoints;
        float smallAngle = 0.0005f;
        
        for (const auto& element : proximateCorner) {
            float angle = atan2(element.y - playerPos.y, element.x - playerPos.x);
            lightPoints.push_back(getRayImpact(playerPos, angle, maze, tileWidth));
            
            angle -= smallAngle;
            lightPoints.push_back(getRayImpact(playerPos, angle, maze, tileWidth));
            
            angle += 2 * smallAngle;
            lightPoints.push_back(getRayImpact(playerPos, angle, maze, tileWidth));
        }
        return lightPoints;
    }
    
    std::vector<sf::Vector2f> listProximateCorner(const std::vector<std::vector<int>>& maze, 
                                                   sf::Vector2f pos, int tileWidth) {
        std::vector<sf::Vector2f> returnArray;
        int distMax = 8;
        
        int posI = static_cast<int>(pos.x / tileWidth);
        int posJ = static_cast<int>(pos.y / tileWidth);
        
        for (int j = posJ - distMax; j <= posJ + distMax; ++j) {
            for (int i = posI - distMax; i <= posI + distMax; ++i) {
                if (i < 0 || j < 0 || j >= maze.size() || i >= maze[0].size()) continue;
                
                if (maze[j][i] == 1) {
                    float x = i * tileWidth;
                    float y = j * tileWidth;
                    
                    returnArray.push_back(sf::Vector2f(x, y));
                    returnArray.push_back(sf::Vector2f(x + tileWidth, y));
                    returnArray.push_back(sf::Vector2f(x + tileWidth, y + tileWidth));
                    returnArray.push_back(sf::Vector2f(x, y + tileWidth));
                }
            }
        }
        return returnArray;
    }
    
    std::vector<sf::Vector2f> sortPoint(std::vector<sf::Vector2f> listOfPoint, sf::Vector2f pos) {
        auto compareAngles = [pos](const sf::Vector2f& a, const sf::Vector2f& b) {
            float angleA = std::atan2(a.y - pos.y, a.x - pos.x);
            float angleB = std::atan2(b.y - pos.y, b.x - pos.x);
            return angleA < angleB;
        };
        
        std::sort(listOfPoint.begin(), listOfPoint.end(), compareAngles);
        return listOfPoint;
    }
};

