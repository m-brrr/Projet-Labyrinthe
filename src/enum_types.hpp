#pragma once

enum class SpellType {Arrow, BlueFireBall, RedFireBall, PinkFireBall, BlueLightnings, WhiteLightnings};

enum class CharacterType { 
							Aguy, Child, CowBoy, Doctor, DogWarrior, Elf, Farmer, Groom, 
							Indian, Knight, Lezard, Orc, Princess, Skeleton, Spartiate, Thief, Warrior, Woman, Zombie
						};

enum class Direction {Up, Down, Left, Right};


inline int convertToNumberCharacter (Direction maDirection){
	switch (maDirection){
		case Direction::Down : return 0;
		case Direction::Left : return 1;
		case Direction::Right : return 2;
		case Direction::Up : return 3;
	}
	return -1;
}

inline int convertToNumberSpell (Direction maDirection){
	switch (maDirection){
		case Direction::Down : return 0;
		case Direction::Left : return 1;
		case Direction::Right : return 3;
		case Direction::Up : return 2;
	}
	return -1;
}

inline sf::Vector2f getNewPosition(float dt,sf::Vector2f formerAbsPosition, float speed, Direction maDirection){	//prend une position absolue et renvoie la nouvelle position absolue
			switch (maDirection){
				case Direction::Right : return formerAbsPosition+(sf::Vector2f(speed*dt, 0));
				case Direction::Left : return formerAbsPosition+(sf::Vector2f(-speed*dt, 0)); 
				case Direction::Up : return formerAbsPosition+(sf::Vector2f(0, -speed*dt)); 
				case Direction::Down : return formerAbsPosition+(sf::Vector2f(0, +speed*dt));
			}
			throw std::runtime_error("Direction invalide");
		}

inline Direction determinerDirection(float diffX, float diffY) {
	if (abs(diffY)>=abs(diffX)){
		if (diffY>=0) return Direction::Down;
		else  return Direction::Up;
	}
	else {
		if (diffX>=0) return Direction::Right;
		else return Direction::Left;
	}
}

enum class EnemyState {
	Patrol,
	Chase,
	Search
};

enum class StatesNames {
	Menu, Game, GameOver, Pause, HappyEnd, Settings, Character
};

enum class SoundEffectNames {
	BruitPas, BruitLancerSort, BruitExplosionSort, BruitSouffle, BruitMortEnemi, Click
};

enum class MusicNames {
	MusicMenu, MusicGame
};