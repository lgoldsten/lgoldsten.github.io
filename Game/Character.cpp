// character.cpp
// Contains functions for a character's actions

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>
#include <math.h>
#include "Character.h"
#include "Enemy.h"
#include "Map.h"

const int d20 = 19;
const int max_name_size = 10;
const int max_enemy_name_size = 10;

Character::Character() {
	max_hp = 10;
	curr_hp = max_hp;
	AC = 16;
	m_atk_bonus = 3;
	m_dmg_bonus = 3;
	initiative = 20;
	init_roll = 0;
	move_speed = 6;
	index = -1;
	name = "unnamed_hero";
}

//Character::~Character() {}

// attempts to hit enemy with melee weapon and if successful, deals damage
int Character::m_attack_target(int enemy_AC) 
{
	int atk_roll = 1 + rand()%d20;
	std::cout << " with their " << melee_weapon.get_name() << std::endl;
	std::cout << name << " rolls " << (atk_roll + m_atk_bonus + melee_weapon.get_atk_bonus()) << " (" << atk_roll << "/20 + ";
	std::cout << m_atk_bonus + melee_weapon.get_atk_bonus() << " mod)" << std::endl;

	if ((atk_roll + m_atk_bonus + melee_weapon.get_atk_bonus()) > enemy_AC) {
		int dmg_roll = 1 + rand()%(melee_weapon.get_dmg_dice() - 1);
		std::cout << name << " hits for " << (dmg_roll + m_dmg_bonus + melee_weapon.get_dmg_bonus()) << " damage! (" << dmg_roll;
		std::cout << "/" << melee_weapon.get_dmg_dice() << " + " << m_dmg_bonus + melee_weapon.get_dmg_bonus() << " mod)" << std::endl;
		return (dmg_roll + m_dmg_bonus + melee_weapon.get_dmg_bonus());
	}
	else {
		std::cout << name << " misses!" << std::endl;
		return 0;
	}
}

// attempts to hit enemy with ranged weapon and if successful, deals damage
int Character::r_attack_target(int enemy_AC) 
{
	int atk_roll = 1 + rand()%d20;
	std::cout << " with their " << ranged_weapon.get_name() << std::endl;
	std::cout << name << " rolls " << (atk_roll + r_atk_bonus + ranged_weapon.get_atk_bonus()) << " (" << atk_roll << "/20 + ";
	std::cout << r_atk_bonus + ranged_weapon.get_atk_bonus() << " mod)" << std::endl;

	if ((atk_roll + r_atk_bonus + ranged_weapon.get_atk_bonus()) > enemy_AC) {
		int dmg_roll = 1 + rand()%(melee_weapon.get_dmg_dice() - 1);
		std::cout << name << " hits for " << (dmg_roll + r_dmg_bonus + ranged_weapon.get_dmg_bonus()) << " damage! (" << dmg_roll;
		std::cout << "/" << ranged_weapon.get_dmg_dice() << " + " << r_dmg_bonus + ranged_weapon.get_dmg_bonus() << " mod)" << std::endl;
		return (dmg_roll + r_dmg_bonus + ranged_weapon.get_dmg_bonus());
	}
	else {
		std::cout << name << " misses!" << std::endl;
		return 0;
	}
}

int Character::begin_turn(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground, bool moved) 
{

	bool valid_input = 1; 
	int player_choice = 0;
	int enemies_killed = 0;
	int attack_checker = 0;
	
	print_display(Enemies, Characters, 4, battleground);
	std::cout << std::endl << name << "'s Turn" << std::endl;
	std::cout << "1. Move" << std::endl;
	std::cout << "2. Attack" << std::endl;
	std::cout << "3. Do Nothing" << std::endl;

	while (valid_input != 0) {
		
		std::cin >> player_choice;
		if (player_choice > 0 and player_choice < 4) {
			valid_input = 0; // input is valid
		}
		else { 
			std::cin.clear(); 
			std::cin.ignore(); 
			print_display(Enemies, Characters, 5, battleground);
			std::cout << std::endl << name << "'s Turn" << std::endl;
			std::cout << "1. Move" << std::endl;
			std::cout << "2. Attack" << std::endl;
			std::cout << "3. Do Nothing" << std::endl;
			std::cout << "Invalid Input" << std::endl;
			std::this_thread::sleep_for(std::chrono::duration<double>(0.2));
			print_display(Enemies, Characters, 4, battleground);
			std::cout << std::endl << name << "'s Turn" << std::endl;
			std::cout << "1. Move" << std::endl;
			std::cout << "2. Attack" << std::endl;
			std::cout << "3. Do Nothing" << std::endl;
		}
	}

	if (player_choice == 1) {
		move(Enemies, Characters, battleground);
		if (moved != 0) { 
			enemies_killed += begin_turn(Enemies, Characters, battleground, 0); 
		}
	} 
	else if (player_choice == 2) {
		attack_checker = begin_attack(Enemies, Characters, battleground);
		if (attack_checker != - 1) {
			enemies_killed += attack_checker;
		}
		else { enemies_killed += begin_turn(Enemies, Characters, battleground, moved); }
	} 
	else if (player_choice == 3) {
		print_display(Enemies, Characters, 0, battleground);
	}

	if (moved != 0 and attack_checker != -1) {
		std::cout << std::endl << "Press Enter to Continue ";
		std::cin.ignore();
		std::cin.ignore();
	}

	return enemies_killed;
}

void Character::move(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground) 
{
	
	double distance_traveled = 0;

	while (distance_traveled < move_speed) {

		std::string direction;
		int distance;

		print_display(Enemies, Characters, 1, battleground);
		std::cout << "Movement Left: " << move_speed - distance_traveled << std::endl;
		std::cout << "Please enter direction and distance" << std::endl; 
		std::cin.clear();
		std::cin >> direction;
		if (direction == "end") {
			print_display(Enemies, Characters, 0, battleground);
			break;
		}
		std::cin >> distance;
		
		if (direction != "up" and direction != "u" and direction != "right" and direction != "r" and
			direction != "down" and direction != "d" and direction != "left" and direction != "l" and 
			direction != "up-left" and direction != "left-up" and direction != "ul" and direction != "lu" and 
			direction != "up-right" and direction != "right-up" and direction != "ur" and direction != "ru" and 
			direction != "down-left" and direction != "left-down" and direction != "dl" and direction != "ld" and 
			direction != "down-right" and direction != "right-down" and direction != "dr" and direction != "rd") {
			
			print_display(Enemies, Characters, 1, battleground);
			std::cout << "Invalid Direction" << std::endl;
			std::cout << std::endl << "Press Enter to Continue ";
			std::cin.ignore();
			std::cin.ignore();
		}

		if ((distance + distance_traveled) > move_speed) { 
			distance = 0; // negates illegal move
		}

		if (direction == "up" or direction == "u") {
			distance_traveled += move_helper(Enemies, Characters, battleground, 0, -distance);
		}
		else if (direction == "down" or direction == "d") {
			distance_traveled += move_helper(Enemies, Characters, battleground, 0, distance);
		}
		else if (direction == "left" or direction == "l") {
			distance_traveled += move_helper(Enemies, Characters, battleground, -distance, 0);
		}
		else if (direction == "right" or direction == "r") {
			distance_traveled += move_helper(Enemies, Characters, battleground, distance, 0);
		}
		else if (direction == "up-left" or direction == "left-up" or direction == "ul" or direction == "lu") {
			distance_traveled += move_helper(Enemies, Characters, battleground, -distance, -distance);
		}
		else if (direction == "up-right" or direction == "right-up" or direction == "ur" or direction == "ru") {
			distance_traveled += move_helper(Enemies, Characters, battleground, distance, -distance);
		}
		else if (direction == "down-left" or direction == "left-down" or direction == "dl" or direction == "ld") {
			distance_traveled += move_helper(Enemies, Characters, battleground, -distance, distance);
		}
		else if (direction == "down-right" or direction == "right-down" or direction == "rd" or direction == "dr") {
			distance_traveled += move_helper(Enemies, Characters, battleground, distance, distance);
		}
	}
}

double Character::move_helper(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground, int distance_x, int distance_y) 
{

	int original_x = battleground->get_char_x(index);
	int original_y = battleground->get_char_y(index);
	int longer_side;
	double distance_traveled = 0;
	double temp;


	if (original_x + distance_x < battleground->get_width() and original_x + distance_x > 0 and 
		original_y + distance_y < battleground->get_height() and original_y + distance_y > 0) {

		if (std::abs(distance_x) > std::abs(distance_y)) { longer_side = std::abs(distance_x); 
		} else { longer_side = std::abs(distance_y); }

		temp = distance_traveled;

		for (int i = 1; i < longer_side + 1; i++) {

			int move_checker = 0; // checks for amount of movement
			int current_x = battleground->get_char_x(index);
			int current_y = battleground->get_char_y(index);

			// movement to the right
			if (current_x < original_x + distance_x and distance_x > 0) {
					battleground->set_char_x(index, current_x + 1);
					current_x += 1;
					move_checker++;
			}
			// movement to the left
			if (current_x > original_x + distance_x and distance_x < 0) {
					battleground->set_char_x(index, current_x - 1);
					current_x -= 1;
					move_checker++;
			}
			// movement upwards
			if (current_y > original_y + distance_y and distance_y < 0) {

					battleground->set_char_y(index, current_y - 1);
					current_y -= 1;
					move_checker++;
			}
			// movement downwards
			if (current_y < original_y + distance_y and distance_y > 0) {
					battleground->set_char_y(index, current_y + 1);
					current_y += 1;
					move_checker++;
			}

			if (battleground->get_map_data(current_x, current_y) != ' ') {
					battleground->set_char_x(index, original_x);
					battleground->set_char_y(index, original_y);
					distance_traveled = temp;
					print_display(Enemies, Characters, 1, battleground); 
					std::cout << "Invalid Move" << std::endl;
					std::cout << std::endl << "Press Enter to Continue ";
					std::cin.ignore();
					std::cin.ignore();
					break;
			}
			else {
				print_display(Enemies, Characters, 1, battleground);
				std::cout << name << " is moving..." << std::endl << std::endl;
				std::this_thread::sleep_for(std::chrono::duration<double>(0.25));
				if (move_checker == 2)      { distance_traveled += 1.5; }
				else if (move_checker == 1) { distance_traveled += 1.0; }
				else 				        { distance_traveled += 0.0; }
			}
		}
	} 
	else { 
		distance_traveled = 0;
		print_display(Enemies, Characters, 1, battleground); 
		std::cout << "Invalid Move" << std::endl;
		std::cout << std::endl << "Press Enter to Continue ";
		std::cin.ignore();
		std::cin.ignore();
	}


	print_display(Enemies, Characters, 0, battleground);
	return distance_traveled;
}

// returns enemies killed
int Character::begin_attack(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground) 
{

	bool valid_input = 1;
	int selection = 0;
	
	if (ranged_weapon.get_type() != "ranged") {
		return melee_attack(Enemies, Characters, battleground);
	}

	print_display(Enemies, Characters, 2, battleground); 
	std::cout << "Melee or Ranged?" << std::endl;
	std::cout << "1. Melee  (" << melee_weapon.get_name() << ")" << std::endl;
	std::cout << "2. Ranged (" << ranged_weapon.get_name() << ")" << std::endl;

	while (valid_input != 0) {
		selection = 0;
		std::cin >> selection;

		if (selection == 1 or selection == 2) {
			valid_input = 0;
		}
		else { 
			print_display(Enemies, Characters, 3, battleground); 
			std::cout << "Melee or Ranged?" << std::endl;
			std::cout << "1. Melee  (" << melee_weapon.get_name() << ")" << std::endl;
			std::cout << "2. Ranged (" << ranged_weapon.get_name() << ")" << std::endl;
			std::cout << "Invalid Input" << std::endl;
			std::this_thread::sleep_for(std::chrono::duration<double>(0.25));
			print_display(Enemies, Characters, 2, battleground);
			std::cout << "Melee or Ranged?" << std::endl;
			std::cout << "1. Melee  (" << melee_weapon.get_name() << ")" << std::endl;
			std::cout << "2. Ranged (" << ranged_weapon.get_name() << ")" << std::endl;
		}
	}

	if (selection == 2) {
		return ranged_attack(Enemies, Characters, battleground);
	}
	return melee_attack(Enemies, Characters, battleground);
}


int Character::melee_attack(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground)
{
	int selection;
	bool valid_input = 1;
	int targets[Enemies.size()];
	int total_enemies = Enemies.size(); // represents the # of enemies when the fight starts
	int non_targets = 0;
	int possible_targets = 0;

	for (int i = 0; i < int(Enemies.size()); i++) {
		targets[i] = i;
		if (Enemies[i]->get_curr_hp() > 0 and battleground->check_adjacency(index, i) == 1) { possible_targets++; }
	}

	if (possible_targets == 0) {
		print_display(Enemies, Characters, 1, battleground);
		std::cout << "No possible targets" << std::endl;
		std::cout << std::endl << "Press Enter to Continue ";
		std::cin.ignore();
		std::cin.ignore();
		return -1;
	}

	print_display(Enemies, Characters, possible_targets + 1, battleground);

	// display targets
	std::cout << "Choose a target to attack" << std::endl;
	int target_num = 1;
	for (int i = 0; i < int(Enemies.size()); i++) {
		if (battleground->check_adjacency(index, i) == 1 and Enemies[i]->get_curr_hp() > 0) {
				std::cout << target_num << ". " << Enemies[i]->get_name();
				int num_spaces = max_enemy_name_size - Enemies[i]->get_name().length();
				for (int s = 0; s < num_spaces; s++) { std::cout << " "; };
				std::cout << " (" << Enemies[i]->get_curr_hp() << " hp)" << std::endl;
				target_num++;
		}
		else {
			for (int j = i - non_targets; j < total_enemies; j++) {
				targets[j]++;
			}
			non_targets++;
		}
	}
	std::cout << target_num << ". " << "Cancel" << std::endl;

	// choose a target
	while (valid_input == 1) {
		selection = 0;
		std::cin >> selection;

		// check for valid input
		if (selection <= (total_enemies - non_targets + 1) && selection > 0) {

			if (selection == target_num) { return -1; }

			valid_input = 0; // input is valid
			int target_index = targets[selection-1];
			print_display(Enemies, Characters, 3, battleground);
			std::cout << name << " attacks " << Enemies[target_index]->get_name();

			// attack
			Enemies[target_index]->recieve_dmg(m_attack_target(Enemies[target_index]->get_AC()));

			// check if enemy is dead
			if (Enemies[target_index]->get_curr_hp() <= 0) {
				battleground->set_enem_x(target_index, 0);
				battleground->set_enem_y(target_index, 0);
				return 1; // increments dead enemies
			}
		}
	}
	return 0; // no enemies killed
}

int Character::ranged_attack(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground) 
{
	
	int selection;
	bool valid_input = 1;
	int targets[Enemies.size()];
	int total_enemies = Enemies.size(); // represents the # of enemies when the fight starts
	int non_targets = 0;
	int possible_targets = 0;

	for (int i = 0; i < int(Enemies.size()); i++) {
		targets[i] = i;
		if (Enemies[i]->get_curr_hp() > 0 and check_range_and_cover(battleground, i) == 0) { 
			possible_targets++; 
		}
	}

	if (possible_targets == 0) {
		print_display(Enemies, Characters, 1, battleground);
		std::cout << "No possible targets" << std::endl;
		std::cout << std::endl << "Press Enter to Continue ";
		std::cin.ignore();
		std::cin.ignore();
		return -1;
	}

	print_display(Enemies, Characters, possible_targets + 1, battleground);

	// display targets
	std::cout << "Choose a target to attack" << std::endl;
	int target_num = 1;
	for (int i = 0; i < int(Enemies.size()); i++) {
		if (Enemies[i]->get_curr_hp() > 0 and check_range_and_cover(battleground, i) == 0) {
				std::cout << target_num << ". " << Enemies[i]->get_name();
				int num_spaces = max_enemy_name_size - Enemies[i]->get_name().length();
				for (int s = 0; s < num_spaces; s++) { std::cout << " "; };
				std::cout << " (" << Enemies[i]->get_curr_hp() << " hp)" << std::endl;
				target_num++;
		}
		else {
			for (int j = i - non_targets; j < total_enemies; j++) {
				targets[j]++;
			}
			non_targets++;
		}
	}
	std::cout << target_num << ". " << "Cancel" << std::endl;

	// choose a target
	while (valid_input == 1) {
		selection = 0;
		std::cin >> selection;

		// check for valid input
		if (selection <= (total_enemies - non_targets + 1) && selection > 0) {

			if (selection == target_num) { return -1; }

			valid_input = 0; // input is valid
			int target_index = targets[selection-1];
			print_display(Enemies, Characters, 3, battleground);
			std::cout << name << " attacks " << Enemies[target_index]->get_name();

			// attack
			Enemies[target_index]->recieve_dmg(r_attack_target(Enemies[target_index]->get_AC()));

			// check if enemy is dead
			if (Enemies[target_index]->get_curr_hp() <= 0) {
				battleground->set_enem_x(target_index, 0);
				battleground->set_enem_y(target_index, 0);
				return 1; // increments dead enemies
			}
		}
	}
	return 0; // no enemies killed
}

// checks that a target is within range and line-of-sight
bool Character::check_range_and_cover(Map *battleground, int enemy_index) 
{
	bool attackable = 1;
	double my_x = (double)battleground->get_char_x(index);
	double my_y = (double)battleground->get_char_y(index);
	double enemy_x = (double)battleground->get_enem_x(enemy_index);
	double enemy_y = (double)battleground->get_enem_y(enemy_index);

	if (battleground->check_adjacency(index, enemy_index) == 1) {
		return attackable; // adjacent target
	}

	double distance = sqrt(pow(my_x - enemy_x, 2) + pow(my_y - enemy_y, 2));
	if (distance > (double)ranged_weapon.get_range()) {
		return attackable; // out of range
	}

	attackable = check_cover(my_x, my_y, enemy_x, enemy_y, battleground);
	return attackable;
}

bool Character::check_cover(double x1, double y1, double x2, double y2, Map *battleground) 
{	
  // Bresenham's line algorithm
  
  const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
  if(steep)
  {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }
 
  if(x1 > x2)
  {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }
 
  const float dx = x2 - x1;
  const float dy = fabs(y2 - y1);
 
  float error = dx / 2.0f;
  const int ystep = (y1 < y2) ? 1 : -1;
  int y = (int)y1;
 
  const int maxX = (int)x2;
 
  for(int x=(int)x1; x<maxX; x++)
  {
  	if (x == x1 and y == y1) { /* character location, do nothing */ }
  	else if (x == x2 and y == y2) { /* enemy location, do nothing */ }
    else if(steep)
    {
        if (battleground->get_map_data(y, x) != ' ') { return true; } // target is blocked
    }
    else
    {
        if (battleground->get_map_data(x, y) != ' ') { return true; } // target is blocked
    }
 
    error -= dy;
    if(error < 0)
    {
        y += ystep;
        error += dx;
    }
  }

  return false;
}

void Character::print_display(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, int lower_area, 
	Map *battleground) 
{

	int biggest_group;

	if (Characters.size() > Enemies.size()) {
		biggest_group = Characters.size();
	}
	else {
		biggest_group = Enemies.size();
	}

	// clears screen
	for (int i = 0; i < 50; i++) { std::cout << '\n'; } 

	// prints upper display containing current hp of everyone in the battle
	for (int i = 0; i < biggest_group; i++) {

		if (i < int(Characters.size())) {
			int length = Characters[i]->get_name().length(); 
			int char_hp = Characters[i]->get_curr_hp();

			if (char_hp > 99)       { length += 3; }
			else if (char_hp > 9)   { length += 2; }
			else if (char_hp >= 0)  { length += 1; }
			else if (char_hp > -10) { length += 2; }
			else 				    { length += 3; }

			std::cout << Characters[i]->get_name() << ": ";

			// + 2 in starting i value in below loop is due to the above string => ": "
			for (int i = max_name_size - length + 2; i > 0; i--) { std::cout << " "; };

			std::cout << char_hp << " hp   |   ";
			
		}
		else { std::cout << "                    |   "; }

		if (i < int(Enemies.size())) {
			int length = Enemies[i]->get_name().length(); 
			int enem_hp = Enemies[i]->get_curr_hp();

			if (enem_hp > 99)       { length += 3; }
			else if (enem_hp > 9)   { length += 2; }
			else 				    { length += 1; }

			std::cout << Enemies[i]->get_name() << ": ";

			// + 2 in starting i value in below loop is due to the above string => ": "
			for (int i = max_enemy_name_size - length + 2; i > 0; i--) { std::cout << " "; };

			if (enem_hp > 0) { 
				std::cout << enem_hp << " hp     " << std::endl; 
			} else {
				std::cout << "Dead     " << std::endl; 
			}
			
		}
		else { std::cout << std::endl; }

	}

	std::cout << std::endl;
	battleground->draw_map();
	// prints the space in between map and lower displays (22 being the default terminal window height - 2 spaces)
	for (int i = 0; i < (21 - biggest_group - lower_area - battleground->get_height()); i++) { std::cout << std::endl; }
}