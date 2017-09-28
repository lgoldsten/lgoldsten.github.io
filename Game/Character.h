// Character.h
// Contains information for a given character, like stats, abilities, and items

#pragma once
#include <string>
#include <vector>
#include "Map.h"
#include "Weapon.h"

class Enemy;

class Character {
public:
	Character();
	//~Character();
	
	void recieve_dmg(int damage)   		{ curr_hp -= damage; }
	int get_curr_hp() 			   		{ return curr_hp; }
	int get_AC() 				   		{ return AC; }
	int get_init_roll() 		   		{ return init_roll; }
	void set_init_roll(int roll)   		{ init_roll = roll + initiative; }
	int get_init_mod() 			   		{ return initiative; }
	void set_init_mod(int init)         { initiative = init; }
	int get_move_speed() 		   		{ return move_speed; }
	void set_move_speed(int speed) 		{ move_speed = speed; }
	int get_index() 			        { return index; }
	void set_index(int num) 	        { index = num; }
	std::string get_name() 	            { return name; }
	void set_name(std::string new_name) { name = new_name; }
	void equip_weapon(Weapon new_weapon) { if (new_weapon.get_type() == "melee") { melee_weapon = new_weapon; } else { ranged_weapon = new_weapon; } }
	int begin_turn(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground, bool moved); // character takes turn, returns enemies killed

private:
	int max_hp;
	int curr_hp; 
	int AC; // armor class (how difficult it is to hit someone)
	int m_atk_bonus; // bonus to hit for melee
	int m_dmg_bonus; // bonus to damage for melee
	int r_atk_bonus; // bonus to hit for ranged
	int r_dmg_bonus; // bonus to damage for ranged
	int initiative; // bonus to init_roll
	int init_roll;  // determines turn order
	int move_speed; 
	int index;
	Weapon melee_weapon;
	Weapon ranged_weapon;
	std::string name;
	double move_helper(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground, int distance_x, int distance_y);
	int m_attack_target(int enemy_AC); // attack target with melee weapon
	int r_attack_target(int enemy_AC); // attack target with ranged weapon
	void move(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground); // character moves
	int begin_attack(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground); // character chooses and attack a target, returns enemies killed
	int melee_attack(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground);
	int ranged_attack(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground);
	bool check_range_and_cover(Map *battleground, int enemy_index);
	bool check_cover(double x0, double y0, double x1, double y1, Map *battleground);
	void print_display(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, int lower_area, Map *battleground);
};

