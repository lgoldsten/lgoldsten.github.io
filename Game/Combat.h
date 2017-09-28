// Combat.h
// Contains information for a given combat, like turn order

#pragma once
#include <vector>
#include "Character.h"
#include "Enemy.h"

class Combat {
public:
	//Combat();
	//~Combat();
	void start_combat(std::vector<Character*> Characters, std::vector<Enemy*> Enemies, Map *battleground);
private:
	//std::vector<int> turn_order;
	//std::vector<int> init_rolls;
};
