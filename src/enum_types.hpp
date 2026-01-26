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

enum class EnemyState {
	Patrol,
	Chase,
	Search
};

enum class StatesNames {
	Menu, Game, GameOver, Pause
};