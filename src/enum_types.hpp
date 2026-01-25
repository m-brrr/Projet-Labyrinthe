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

inline sf::Vector2f spellProduction(Direction maDirection) {
	switch(maDirection) {
		case Direction::Down : return sf::Vector2f(400.f,365.f);
		case Direction::Left : return sf::Vector2f(335.f, 300.f);
		case Direction::Right : return sf::Vector2f(465.f, 300.f);
		case Direction::Up : return sf::Vector2f(400.f, 235.f);
	}
	throw "this is an exception";
}

enum class EnemyState {
	Patrol,
	Chase,
	Search
};

enum class StatesNames {
	Menu, Game, GameOver, Pause
};