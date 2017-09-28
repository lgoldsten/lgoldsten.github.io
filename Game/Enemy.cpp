// Enemy.cpp
// Contains functions for an enemy's actions

#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

#include "Enemy.h"
#include "Character.h"

const int d20 = 20;
const int max_name_size = 10;
const int max_enemy_name_size = 10;

Enemy::Enemy() 
{
	max_hp = 10;
	curr_hp = max_hp;
	AC = 13;
	atk_bonus = 1;
	dmg_bonus = 1;
	dmg_dice = 6;
	initiative = 1;
	init_roll = 0;
	move_speed = 6;
	index = -1;
	name = "unnamed_enemy";
	type = "melee";
}

//Enemy::~Enemy() {}

// attempts to hit character with melee weapon and if successful, deals damage
int Enemy::m_attack(int hero_AC) 
{
	int atk_roll = rand()%d20;

	if ((atk_roll + atk_bonus) > hero_AC) {
		int dmg_roll = rand()%dmg_dice;
		std::cout << name << " hit for " << (dmg_roll + dmg_bonus) << "!" << std::endl << std::endl;
		return (dmg_roll + dmg_bonus);
	}
	else {
		std::cout << name << " missed!" << std::endl << std::endl;
		return 0;
	}
}

// attempts to hit character with ranged weapon and if successful, deals damage
int Enemy::r_attack(int hero_AC) 
{
	int atk_roll = rand()%d20;

	if ((atk_roll + atk_bonus) > hero_AC) {
		int dmg_roll = rand()%dmg_dice;
		std::cout << name << " hit for " << (dmg_roll + dmg_bonus) << "!" << std::endl << std::endl;
		return (dmg_roll + dmg_bonus);
	}
	else {
		std::cout << name << " missed!" << std::endl << std::endl;
		return 0;
	}
}

void Enemy::recieve_dmg(int damage) { curr_hp -= damage; }

int Enemy::get_curr_hp() { return curr_hp; }

int Enemy::get_AC() { return AC;}

void Enemy::set_init_roll(int roll) { init_roll = roll + initiative; }

int Enemy::get_init_roll() { return init_roll; }

int Enemy::get_init_mod() { return initiative; }

int Enemy::get_move_speed() { return move_speed; }
 
void Enemy::set_move_speed(int speed) { move_speed = speed; }

int Enemy::get_index() { return index; }

void Enemy::set_index(int num) { index = num; }

std::string Enemy::get_name() { return name; }

void Enemy::set_name(std::string new_name) { name = new_name; }

int Enemy::begin_turn(std::vector<Character*> Characters, std::vector<Enemy*> Enemies, Map *battleground) 
{
	int targets[Characters.size()];
	int total_characters = Characters.size(); // represents the # of enemies when the fight starts
	int characters_killed = 0;
	int possible_targets = 0;
	for (int i = 0; i < int(Characters.size()); i++) {
		targets[i] = i;
	}

	for (int i = 0; i < 50; i++) { std::cout << '\n'; } // clears screen
	print_display(Enemies, Characters, 2, battleground);
	std::cout << std::endl << name << "'s Turn" << std::endl << std::endl;
	std::this_thread::sleep_for(std::chrono::duration<double>(0.5));

	// check for possible targets
	for (int i = 0; i < total_characters; i++) {
		if (type == "melee" and Characters[i]->get_curr_hp() > 0 and battleground->check_adjacency(i, index) == 1) {
			possible_targets++;	
		}
	}

	if (possible_targets == 0) {
		move(Enemies, Characters, battleground);
		// check for possible targets
		for (int i = 0; i < total_characters; i++) {
			if (Characters[i]->get_curr_hp() > 0 and battleground->check_adjacency(i, index) == 1) {
				possible_targets++;	
			}
		}
		if (possible_targets == 0) { move(Enemies, Characters, battleground); }
		else { characters_killed = begin_m_attack(Enemies, Characters, battleground); }

	} else { 
		characters_killed = begin_m_attack(Enemies, Characters, battleground); 
	}
	
	std::cout << "Press Enter to Continue ";
	std::cin.ignore();
	return characters_killed;
}

void Enemy::move(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground) 
{	
	int width = battleground->get_width();
	int height = battleground->get_height();
	int starting_x = battleground->get_enem_x(index);
	int starting_y = battleground->get_enem_y(index);
	double shortest_path_distance = 1000;
	int shortest_path_x;
	int shortest_path_y;
	
	std::vector< std::vector<double> > *map_copy; 
	map_copy = new std::vector< std::vector<double> >(width, std::vector<double>(height));


	for (int i = 0; i < battleground->get_height(); i++) {	
		for (int j = 0; j < battleground->get_width(); j++) {
			(*map_copy)[j][i] = 1000;
		}
	}

	(*map_copy)[starting_x][starting_y] = 0;
	*map_copy = shortest_path_helper(battleground, *map_copy, starting_x, starting_y);

	for (int i = 0; i < int(Characters.size()); i++) {
		int x = battleground->get_char_x(i);
		int y = battleground->get_char_y(i);

		if ((*map_copy)[x + 1][y] < shortest_path_distance) {
			shortest_path_distance = (*map_copy)[x + 1][y];
			shortest_path_x = x + 1;
			shortest_path_y = y;

		}
		if ((*map_copy)[x - 1][y] < shortest_path_distance) {
			shortest_path_distance = (*map_copy)[x - 1][y];
			shortest_path_x = x - 1;
			shortest_path_y = y;
		}
		if ((*map_copy)[x][y + 1] < shortest_path_distance) {
			shortest_path_distance = (*map_copy)[x][y + 1];
			shortest_path_x = x;
			shortest_path_y = y + 1;
		}
		if ((*map_copy)[x][y - 1] < shortest_path_distance) {
			shortest_path_distance = (*map_copy)[x][y - 1];
			shortest_path_x = x;
			shortest_path_y = y - 1;
		}
		if ((*map_copy)[x + 1][y + 1] < shortest_path_distance) {
			shortest_path_distance = (*map_copy)[x + 1][y + 1];
			shortest_path_x = x + 1;
			shortest_path_y = y + 1;
		}
		if ((*map_copy)[x + 1][y - 1] < shortest_path_distance) {
			shortest_path_distance = (*map_copy)[x + 1][y - 1];
			shortest_path_x = x + 1;
			shortest_path_y = y - 1;
		}
		if ((*map_copy)[x - 1][y + 1] < shortest_path_distance) {
			shortest_path_distance = (*map_copy)[x - 1][y + 1];
			shortest_path_x = x - 1;
			shortest_path_y = y + 1;
		}
		if ((*map_copy)[x - 1][y - 1] < shortest_path_distance) {
			shortest_path_distance = (*map_copy)[x - 1][y - 1];
			shortest_path_x = x - 1;
			shortest_path_y = y - 1;
		}
	}

	if (shortest_path_distance == 1000) {
		return;
	}

	std::vector< std::string > *shortest_path = NULL; 
	shortest_path = prepare_for_move(battleground, *map_copy, starting_x, starting_y, shortest_path_x, shortest_path_y);

	// peform on-screen move of the enemy
	for (int i = int(shortest_path->size()) - 1; i > int(shortest_path->size()) - 1 - move_speed and i >= 0; i--) {
		if ((*shortest_path)[i] == "right") { battleground->set_enem_x(index, battleground->get_enem_x(index) + 1); }
		else if ((*shortest_path)[i] == "left") { battleground->set_enem_x(index, battleground->get_enem_x(index) - 1); }
		else if ((*shortest_path)[i] == "down") { battleground->set_enem_y(index, battleground->get_enem_y(index) + 1); }
		else if ((*shortest_path)[i] == "up") { battleground->set_enem_y(index, battleground->get_enem_y(index) - 1); }
		else if ((*shortest_path)[i] == "right-down") { 
			battleground->set_enem_x(index, battleground->get_enem_x(index) + 1); 
			battleground->set_enem_y(index, battleground->get_enem_y(index) + 1); 
		}
		else if ((*shortest_path)[i] == "left-down") { 
			battleground->set_enem_x(index, battleground->get_enem_x(index) - 1); 
			battleground->set_enem_y(index, battleground->get_enem_y(index) + 1); 
		}
		else if ((*shortest_path)[i] == "right-up") { 
			battleground->set_enem_x(index, battleground->get_enem_x(index) + 1); 
			battleground->set_enem_y(index, battleground->get_enem_y(index) - 1); 
		}
		else if ((*shortest_path)[i] == "left-up") { 
			battleground->set_enem_x(index, battleground->get_enem_x(index) - 1); 
			battleground->set_enem_y(index, battleground->get_enem_y(index) - 1); 
		}
		else { break; }
		print_display(Enemies, Characters, 1, battleground);
		std::cout << name << " is moving..." << std::endl << std::endl;
		std::this_thread::sleep_for(std::chrono::duration<double>(.2));
	}

	print_display(Enemies, Characters, -1, battleground);
	delete map_copy;
	delete shortest_path;

}

std::vector< std::vector<double> > Enemy::shortest_path_helper(Map *battleground, std::vector< std::vector<double> > map_copy, int x, int y) 
{

	// right
	if ((x + 1) < battleground->get_width() - 1) {
		if (battleground->get_map_data(x + 1, y) == ' ' and (map_copy[x][y] + 1) < map_copy[x + 1][y]) {			
			map_copy[x + 1][y] = map_copy[x][y] + 1;
			map_copy = shortest_path_helper(battleground, map_copy, x + 1, y);
		}
	}
	// left
	if ((x - 1) > 0) {
		if (battleground->get_map_data(x - 1, y) == ' ' and (map_copy[x][y] + 1) < map_copy[x - 1][y]) {			
			map_copy[x - 1][y] = map_copy[x][y] + 1;
			map_copy = shortest_path_helper(battleground, map_copy, x - 1, y);
		}
	}
	// down
	if ((y + 1) < battleground->get_height() - 1) {
		if (battleground->get_map_data(x, y + 1) == ' ' and (map_copy[x][y] + 1) < map_copy[x][y + 1]) {			
			map_copy[x][y + 1] = map_copy[x][y] + 1;
			map_copy = shortest_path_helper(battleground, map_copy, x, y + 1);
		}
	}
	// up
	if ((y - 1) > 0) {
		if (battleground->get_map_data(x, y - 1) == ' ' and (map_copy[x][y] + 1) < map_copy[x][y - 1]) {			
			map_copy[x][y - 1] = map_copy[x][y] + 1;
			map_copy = shortest_path_helper(battleground, map_copy, x, y - 1);
		}
	}
	// right-down
	if ((x + 1) < battleground->get_width() - 1 and (y + 1) < battleground->get_height() - 1) {
		if (battleground->get_map_data(x + 1, y + 1) == ' ' and (map_copy[x][y] + 1.5) < map_copy[x + 1][y + 1]) {			
			map_copy[x + 1][y + 1] = map_copy[x][y] + 1.5;
			map_copy = shortest_path_helper(battleground, map_copy, x + 1, y + 1);
		}
	}
	// left-down
	if ((x - 1) > 0 and (y + 1) < battleground->get_height() - 1) {
		if (battleground->get_map_data(x - 1, y + 1) == ' ' and (map_copy[x][y] + 1.5) < map_copy[x - 1][y + 1]) {			
			map_copy[x - 1][y + 1] = map_copy[x][y] + 1.5;
			map_copy = shortest_path_helper(battleground, map_copy, x - 1, y + 1);
		}
	}
	// right-up
	if ((y - 1) > 0 and (x + 1) < battleground->get_width() - 1) {
		if (battleground->get_map_data(x + 1, y - 1) == ' ' and (map_copy[x][y] + 1.5) < map_copy[x + 1][y - 1]) {			
			map_copy[x + 1][y - 1] = map_copy[x][y] + 1.5;
			map_copy = shortest_path_helper(battleground, map_copy, x + 1, y - 1);
		}
	}
	// left-up
	if ((y - 1) > 0 and (x - 1) > 0) {
		if (battleground->get_map_data(x - 1, y - 1) == ' ' and (map_copy[x][y] + 1.5) < map_copy[x - 1][y - 1]) {			
			map_copy[x - 1][y - 1] = map_copy[x][y] + 1.5;
			map_copy = shortest_path_helper(battleground, map_copy, x - 1, y - 1);
		}
	}

	return map_copy;
}


std::vector< std::string >* Enemy::prepare_for_move(Map *battleground, std::vector< std::vector<double> > map_copy, 
	int starting_x, int starting_y, int current_x, int current_y) 
{
	std::vector< std::string > *shortest_path;
	shortest_path = new std::vector< std::string >;

	int width = battleground->get_width();
	int height = battleground->get_height();
	int done = 0;
	double lowest_space;
	int direction;

	// queue up shortest path movements
	while (done == 0) {

		lowest_space = 1000;

		// right
		if ((current_x + 1) < width and lowest_space > (map_copy)[current_x + 1][current_y]) { 
			lowest_space = (map_copy)[current_x + 1][current_y];
			direction = 0;
		}
		// left
		if ((current_x - 1) > 0 and lowest_space > (map_copy)[current_x - 1][current_y]) { 
			lowest_space = (map_copy)[current_x - 1][current_y];
			direction = 1;
		}
		// down
		if ((current_y + 1) < height and lowest_space > (map_copy)[current_x][current_y + 1]) { 
			lowest_space = (map_copy)[current_x][current_y + 1];
			direction = 2;
		}
		// up
		if ((current_y - 1) > 0 and lowest_space > (map_copy)[current_x][current_y - 1]) { 
			lowest_space = (map_copy)[current_x][current_y - 1];
			direction = 3;
		}
		// right-down
		if ((current_x + 1) < width and (current_y + 1) < height and lowest_space > (map_copy)[current_x + 1][current_y + 1]) { 
			lowest_space = (map_copy)[current_x + 1][current_y + 1];
			direction = 4;
		}
		// left-down
		if ((current_x - 1) > 0 and (current_y + 1) < height and lowest_space > (map_copy)[current_x - 1][current_y + 1]) { 
			lowest_space = (map_copy)[current_x - 1][current_y + 1];
			direction = 5;
		}
		// right-up
		if ((current_x + 1) < width and (current_y - 1) > 0 and lowest_space > (map_copy)[current_x + 1][current_y - 1]) {

		 	lowest_space = (map_copy)[current_x + 1][current_y - 1];
		 	direction = 6;
		}
		// left_up
		if ((current_x - 1) > 0 and (current_y - 1) > 0 and lowest_space > (map_copy)[current_x - 1][current_y - 1]) { 
			lowest_space = (map_copy)[current_x - 1][current_y - 1];
			direction = 7;
		}

		switch(direction) {
			case 0 :
				current_x++; 
				shortest_path->push_back("left");
				break;
			case 1 :
				current_x--; 
				shortest_path->push_back("right");
				break;
			case 2 :
				current_y++; 
				shortest_path->push_back("up");
				break;
			case 3 :
				current_y--; 
				shortest_path->push_back("down");
				break;
			case 4 :
				current_x++; 
				current_y++;
				shortest_path->push_back("left-up");
				break;
			case 5 :
				current_x--; 
				current_y++;
				shortest_path->push_back("right-up");
				break;
			case 6 :
				current_x++; 
			 	current_y--;
			 	shortest_path->push_back("left-down");

			 	break;
			case 7 :
				current_x--; 
				current_y--;
				shortest_path->push_back("right-down");
				break;
		}

		if (current_x == starting_x and current_y == starting_y) {
			done = 1;
		}
 	}

	return shortest_path;
}

// returns characters killed
int Enemy::begin_m_attack(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground) 
{
	int selection;
	int targets[Characters.size()];
	int total_characters = Characters.size(); // represents the # of characters when the fight starts
	int non_targets = 0;
	int characters_killed = 0;
	int possible_targets = 0;
	for (int i = 0; i < int(Characters.size()); i++) {
		targets[i] = i;
	}

	for (int i = 0; i < int(Characters.size()); i++) {
		if (Characters[i]->get_curr_hp() > 0 and battleground->check_adjacency(i, index) == 1) {
			possible_targets++;	
		} else {
			for (int j = i - non_targets; j < total_characters; j++) {
				targets[j]++;
			}
			non_targets++;
		}
	}

	if (possible_targets > 0) {
		// enemy chooses target at random
		if (possible_targets == 1) { selection = 0; } // only one target to choose
		else { selection = rand()%(possible_targets - 1); } // random target

		int target_index = targets[selection];

		print_display(Enemies, Characters, 2, battleground);
		std::cout << name << " attacks " << Characters[target_index]->get_name() << std::endl;

		// attack
		Characters[target_index]->recieve_dmg(m_attack(Characters[target_index]->get_AC()));

		// check if target character is dead
		if (Characters[target_index]->get_curr_hp() <= 0) {
			characters_killed = 1; // increments dead characters
		}
	}	

	return characters_killed;
}

// enemy chooses and attacks a target at random within range and returns enemies killed
int begin_r_attack(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, Map *battleground)
{
	int selection;
	int targets[Characters.size()];
	int total_characters = Characters.size(); // represents the # of characters when the fight starts
	int non_targets = 0;
	int characters_killed = 0;
	int possible_targets = 0;
	for (int i = 0; i < int(Characters.size()); i++) {
		targets[i] = i;
	}

	for (int i = 0; i < int(Characters.size()); i++) {
		if (Characters[i]->get_curr_hp() > 0 and check_range_and_cover(battleground, i) == 0) {
			possible_targets++;	
		} else {
			for (int j = i - non_targets; j < total_characters; j++) {
				targets[j]++;
			}
			non_targets++;
		}
	}

	if (possible_targets > 0) {
		// enemy chooses target at random
		if (possible_targets == 1) { selection = 0; } // only one target to choose
		else { selection = rand()%(possible_targets - 1); } // random target

		int target_index = targets[selection];

		print_display(Enemies, Characters, 2, battleground);
		std::cout << name << " attacks " << Characters[target_index]->get_name() << std::endl;

		// attack
		Characters[target_index]->recieve_dmg(r_attack(Characters[target_index]->get_AC()));

		// check if target character is dead
		if (Characters[target_index]->get_curr_hp() <= 0) {
			characters_killed = 1; // increments dead characters
		}
	}	

	return characters_killed;
}

// checks that a target is within range and line-of-sight
bool Enemy::check_range_and_cover(Map *battleground, int char_index) 
{
	bool attackable = 1;
	double my_x = (double)battleground->get_enem_x(index);
	double my_y = (double)battleground->get_enem_y(index);
	double char_x = (double)battleground->get_char_x(char_index);
	double char_y = (double)battleground->get_char_y(char_index);

	if (battleground->check_adjacency(index, char_index) == 1) {
		return attackable; // adjacent target
	}

	double distance = sqrt(pow(my_x - char_x, 2) + pow(my_y - char_y, 2));
	if (distance > (double)ranged_weapon.get_range()) {
		return attackable; // out of range
	}

	attackable = check_cover(my_x, my_y, char_x, char_y, battleground);
	return attackable;
}

bool Enemy::check_cover(double x1, double y1, double x2, double y2, Map *battleground) 
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

void Enemy::print_display(std::vector<Enemy*> Enemies, std::vector<Character*> Characters, int lower_area, 
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
