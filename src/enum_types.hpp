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
		case Direction::Down : return sf::Vector2f(321.f,0.f);
		case Direction::Left : return sf::Vector2f(0.f, 221.f);
		case Direction::Right : return sf::Vector2f(362.f, 301.f);
		case Direction::Up : return sf::Vector2f(401.f, 262.f);

	}
	throw "this is an exception";
}