// Weapon.h
// Contains information related to a given weapon, such as damage, chance to hit, ranged or meleee etc

#pragma once
#include <string>

class Weapon {
public:
	Weapon() 
	{
		range = 1;
		dmg_dice = 3;
		atk_bonus = 0;
		dmg_bonus = 0;
		name = "Fist";
		weapon_type = "melee";
	}
	Weapon(int new_range, int new_dmg_dice, int new_atk_bonus, int new_dmg_bonus, 
		std::string new_name, std::string new_type) 
	{
		range = new_range;
		dmg_dice = new_dmg_dice;
		atk_bonus = new_atk_bonus;
		dmg_bonus = new_dmg_bonus;
		name = new_name;
		weapon_type = new_type;
	}

	std::string get_type()                 { return weapon_type; }
	void set_type(std::string type) { weapon_type = type; }				 
	int get_range() 			           { return range; }
	void set_range(int new_range)          { range = new_range; }
	int get_atk_bonus()                    { return atk_bonus; }
	void set_atk_bonus(int new_atk_bonus)  { atk_bonus = new_atk_bonus; }
	int get_dmg_bonus()                    { return dmg_bonus; }
	void set_dmg_bonus(int new_dmg_bonus)  { dmg_bonus = new_dmg_bonus; }
	int get_dmg_dice()                     { return dmg_dice; }
	void set_dmg_dice(int new_dmg_dice)    { dmg_dice = new_dmg_dice; }
	std::string get_name()                 { return name; }
	void set_name(std::string new_name)    { name = new_name; }

private:
	int range; 
	int dmg_dice;
	int atk_bonus;
	int dmg_bonus;
	std::string name;
	std::string weapon_type;
};