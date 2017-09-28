// Enemy.h
// Contains information for a given enemy, like stats, abilities, and items

#pragma once
#include <string>
#include <vector>
#include "Map.h"

class Character;

class Enemy {
public:
	Enemy();
	//~Enemy();
	int m_attack(int hero_AC); // attacks a hero with melee weapon, returns damage
	int r_attack(int hero_AC); // attacks a hero with ranged weapon, returns damage
	void recieve_dmg(int damage); // enemy takes damage
	int get_curr_hp();
	int get_AC(); 
	void set_atk_bonus(int bonus) { atk_bonus = bonus; }
	int get_init_roll();
	void set_init_roll(int roll);
	int get_init_mod();
	int get_move_speed();
	void set_move_speed(int speed);
	int get_index();
	void set_index(int num);
	std::string get_name();
	void set_name(std::string new_name);
	// enemy takes turn, returns characters killed
	int begin_turn(std::vector<Character*> Characters, std::vector<Enemy*> Enemies, Map *battleground); 
	// enemey moves via finding shortest path to a character
	void move(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground); 
	// enemy chooses and attacks a target at random within range and returns enemies killed
	int begin_m_attack(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground); 
	// enemy chooses and attacks a target at random within range and returns enemies killed
	int begin_r_attack(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground); 
	bool check_range_and_cover(Map *battleground, int char_index);
	bool check_cover(double x1, double y1, double x2, double y2, Map *battleground);
	void print_display(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, int lower_area, Map *battleground);

private:
	int max_hp; // max health
	int curr_hp; // current health
	int AC; // armor class
	int atk_bonus; // bonus to hit
	int dmg_bonus; // bonus to damage
	int dmg_dice;
	int initiative;
	int init_roll;
	int move_speed;
	int index;
	std::string name;
	std::string type; // enemy strategy: melee or ranged
	std::vector< std::vector<double> > shortest_path_helper(Map *battleground, std::vector< std::vector<double> > map_copy, int x, int y);
	std::vector< std::string >* prepare_for_move(Map *battleground, std::vector< std::vector<double> > map_copy, 
											    int starting_x, int starting_y, int current_x, int current_y);
};




