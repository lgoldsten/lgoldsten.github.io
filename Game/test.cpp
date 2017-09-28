#include <iostream>
#include <cstdlib>
#include <time.h>
#include "Character.h"
#include "Enemy.h"
#include "Combat.h"
#include "Map.h"
#include "Weapon.h"

int main() {

	srand(time(NULL));

	std::vector<Character*> Characters(0);
	Weapon longsword_1(1, 8, 1, 1, "+1 Longsword", "melee");
	Weapon shortbow_1 (100, 8, 1, 1, "+1 Shortbow", "ranged");
	

	Character Jeff;
	Jeff.set_name("Jeff");
	Characters.push_back(&Jeff);
	Jeff.set_index(0);
	Jeff.equip_weapon(longsword_1);
	Jeff.equip_weapon(shortbow_1);

	Character Steven;
	Steven.set_name("Steven");
	Characters.push_back(&Steven);
	Steven.set_index(1);
	Steven.equip_weapon(longsword_1);
	Steven.equip_weapon(shortbow_1);

	Character George;
	George.set_name("George");
	Characters.push_back(&George);
	George.set_index(2);
	George.equip_weapon(longsword_1);
	George.equip_weapon(shortbow_1);

	Character Priscilla;
	Priscilla.set_name("Priscilla");
	Characters.push_back(&Priscilla);
	Priscilla.set_index(3);
	Priscilla.equip_weapon(longsword_1);
	Priscilla.equip_weapon(shortbow_1);

	std::vector<Enemy*> Enemies(0);

	Enemy Goblin1;
	Goblin1.set_name("Goblin1");
	Enemies.push_back(&Goblin1);
	Goblin1.set_index(0);


	Enemy Goblin2;
	Goblin2.set_name("Goblin2");
	Enemies.push_back(&Goblin2);
	Goblin2.set_index(1);


	Enemy Goblin3;
	Goblin3.set_name("Goblin3");
	Enemies.push_back(&Goblin3);
	Goblin3.set_index(2);


	Enemy Goblin4;
	Goblin4.set_name("Goblin4");
	Enemies.push_back(&Goblin4);
	Goblin4.set_index(3);

	Combat combat1;

	Map map1(23, 11);
	Map *map_pointer = &map1;
	map1.add_character(3, 3, 'J');
	map1.add_character(3, 4, 'S');
	map1.add_character(3, 6, 'G');
	map1.add_character(3, 7, 'P');
	map1.add_enemy(19, 3, '1');
	map1.add_enemy(19, 4, '2');
	map1.add_enemy(19, 6, '3');
	map1.add_enemy(19, 7, '4');
	
	map1.add_obstacle(1, 1, 'O');
	map1.add_obstacle(1, 2, 'O');
	map1.add_obstacle(2, 1, 'O');

	map1.add_obstacle(21, 1, 'O');
	map1.add_obstacle(20, 1, 'O');
	map1.add_obstacle(21, 2, 'O');

	map1.add_obstacle(1, 9, 'O');
	map1.add_obstacle(2, 9, 'O');
	map1.add_obstacle(1, 8, 'O');

	map1.add_obstacle(21, 9, 'O');
	map1.add_obstacle(20, 9, 'O');
	map1.add_obstacle(21, 8, 'O');

	map1.add_obstacle(11, 5, 'O');
	map1.add_obstacle(12, 5, 'O');
	map1.add_obstacle(10, 5, 'O');
	map1.add_obstacle(11, 4, 'O');
	map1.add_obstacle(11, 6, 'O');
	map1.add_obstacle(13, 5, 'O');
	map1.add_obstacle(9, 5, 'O');
	map1.add_obstacle(11, 3, 'O');
	map1.add_obstacle(11, 7, 'O');
	map1.add_obstacle(12, 6, 'O');
	map1.add_obstacle(12, 4, 'O');
	map1.add_obstacle(10, 6, 'O');
	map1.add_obstacle(10, 4, 'O');

	combat1.start_combat(Characters, Enemies, map_pointer);

	return(0);
}