#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>
#include "enum_types.hpp"

class Son {
	private :

		std::map<SoundEffectNames, sf::SoundBuffer> buffers;
		std::vector<sf::Sound> activeSounds;
		sf::Music backgroundMusic;

		float masterVolume = 100.f;
	    float sfxVolume = 100.f;
	    float musicVolume = 50.f;
	
	public :

		Son() {
	        loadSound(SoundEffectNames::BruitPas, "./assets/sounds/footstep.wav");
	        loadSound(SoundEffectNames::BruitLancerSort, "./assets/sounds/fireball.wav");
	        loadSound(SoundEffectNames::BruitExplosionSort, "./assets/sounds/hit.wav");
			loadSound(SoundEffectNames::BruitMortEnemi, "./assets/sounds/hit.wav");
			loadSound(SoundEffectNames::BruitSouffle, "./assets/sounds/hit.wav");
	        // etc.
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
		void playMusic(const std::string& path, bool loop = true) {
	        if (backgroundMusic.openFromFile(path)) {
	            backgroundMusic.setLoop(loop);
	            backgroundMusic.setVolume(musicVolume);
	            backgroundMusic.play();
	        }
	    }

	    void stopMusic() {
	        backgroundMusic.stop();
	    }

	    void setSFXVolume(float volume) {
	        sfxVolume = std::clamp(volume, 0.f, 100.f);
	    }

	    void setMusicVolume(float volume) {
	        musicVolume = std::clamp(volume, 0.f, 100.f);
	        backgroundMusic.setVolume(musicVolume);
	    }
	};