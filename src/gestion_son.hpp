#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>
#include "enum_types.hpp"
#include <list>

class Son {
	private :

		std::map<SoundEffectNames, sf::SoundBuffer> buffers;
		std::vector<sf::Sound> activeSounds;
		sf::Music menuMusic;
		sf::Music gameMusic;

		float masterVolume = 100.f;
	    float sfxVolume = 100.f;
	    float musicVolume = 50.f;
	
	public :

		Son() {
	        loadSound(SoundEffectNames::BruitPas, "./assets/sons/soundEffects/footsteps.wav");
	        loadSound(SoundEffectNames::BruitLancerSort, "./assets/sons/soundEffects/FireBallCast.wav");
	        loadSound(SoundEffectNames::BruitExplosionSort, "./assets/sons/soundEffects/FireBallImpact.wav");
			loadSound(SoundEffectNames::BruitMortEnemi, "./assets/sons/soundEffects/monsterDie.wav");
			loadSound(SoundEffectNames::Click, "./assets/sons/soundEffects/click.wav");
		
	        // etc.

			if (!menuMusic.openFromFile("./assets/sons/musique/menuMusic.ogg")) {
            throw( "Erreur chargement musique menu");
	        }
	        if (!gameMusic.openFromFile("./assets/sons/musique/GameMusic.ogg")) {
	            throw("Erreur chargement musique jeu");
	        }
			
			activeSounds.reserve(50); // Réserver de la place
		    for (int i = 0; i < 10; i++) {
		        activeSounds.push_back(sf::Sound());
		    }

	        menuMusic.setLoop(true);
	        gameMusic.setLoop(true);
	    	}

	    bool loadSound(SoundEffectNames nom, const std::string& path) {
	        sf::SoundBuffer buffer;
	        if (!buffer.loadFromFile(path)) {
	           throw("Erreur chargement: ressource",path);
	           return false;
	        }
	        buffers[nom] = buffer;
	        return true;
    	}

		void jouerSon(SoundEffectNames nom, float volume = 100.f) {
			
			 if (buffers.find(nom) == buffers.end()) return;
        
	        // Trouver un son disponible ou en créer un nouveau
	        for (auto& sound : activeSounds) {
	            if (sound.getStatus() != sf::Sound::Playing) {
	                sound.setBuffer(buffers[nom]);
	                sound.setVolume(volume * sfxVolume / 100.f);
	                sound.play();
	                return;
	            }
	        }
	        
	        // Si tous les sons sont occupés, en créer un nouveau
	        if (activeSounds.size() < 50) { // Limite arbitraire
	            sf::Sound newSound;
	            newSound.setBuffer(buffers[nom]);
	            newSound.setVolume(volume * sfxVolume / 100.f);
	            newSound.play();
	            activeSounds.push_back(newSound);
	        }
		}

		void playMusic(MusicNames nom) {
			menuMusic.stop();
			gameMusic.stop();
	        
			switch(nom){
				case MusicNames::MusicGame:
					gameMusic.setVolume(musicVolume);
	                gameMusic.play();
	                break;
				case MusicNames::MusicMenu:
					menuMusic.setVolume(musicVolume);
					menuMusic.play();
					break;
			}
	    }

	    void stopAllMusic() {
	        gameMusic.stop();
			menuMusic.stop();
	    }

		void pauseMusic() {
        if (menuMusic.getStatus() == sf::Music::Playing) {
            menuMusic.pause();
        }
        if (gameMusic.getStatus() == sf::Music::Playing) {
            gameMusic.pause();
        }
    }

	    void resumeMusic() {
	        if (menuMusic.getStatus() == sf::Music::Paused) {
	            menuMusic.play();
	        }
	        if (gameMusic.getStatus() == sf::Music::Paused) {
	            gameMusic.play();
	        }
    	}

	    void setSFXVolume(float volume) {
	        sfxVolume = std::clamp(volume, 0.f, 100.f);
	    }

	    void setMusicVolume(float volume) {
	        musicVolume = std::clamp(volume, 0.f, 100.f);
	        menuMusic.setVolume(musicVolume);
	        gameMusic.setVolume(musicVolume);
	    }
	
		
	};