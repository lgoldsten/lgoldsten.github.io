// combat.cpp
// contains functions related to combat

#include <iostream>
#include <cstdlib>
#include "Combat.h"
#include "Map.h"

const int d20 = 20;
const int max_name_size = 10;

int* roll_initiative(std::vector<Character*> Characters, std::vector<Enemy*> Enemies, int turn_order[]);
int core_combat(std::vector<Character*> Characters, std::vector<Enemy*> Enemies, int turn_order[], Map *battleground);

// begins combat
void Combat::start_combat(std::vector<Character*> Characters, std::vector<Enemy*> Enemies, Map *battleground) 
{

	std::cout << "in start combat..." << std::endl;

	int temp[Characters.size() + Enemies.size()];
	int *turn_order = roll_initiative(Characters, Enemies, temp);

	for (int i = 0; i < 50; i++) { std::cout << '\n'; } // clears screen

	std::cout << std::endl << "Combat Start" << std::endl;
	for (int i = 0; i < int(Characters.size() + Enemies.size()); i++) {
		std::cout << i+1 << ". ";
		if (i < 9) { std::cout << " "; }
		if (turn_order[i] < int(Characters.size())) {
			std::cout << Characters[turn_order[i]]->get_name();
			int num_spaces = max_name_size - Characters[turn_order[i]]->get_name().length();
			for (int s = 0; s < num_spaces; s++) { std::cout << " "; };
			std::cout << " (" << Characters[turn_order[i]]->get_init_roll() << ")" << std::endl;
		}
		else {
			std::cout << Enemies[turn_order[i] - Characters.size()]->get_name();
			int num_spaces = max_name_size - Enemies[turn_order[i] - Characters.size()]->get_name().length();
			for (int s = 0; s < num_spaces; s++) { std::cout << " "; };
			std::cout << " (" << Enemies[turn_order[i] - Characters.size()]->get_init_roll(); 
			std::cout << ")" << std::endl;
		}
	}

	for (int i = int(Characters.size()) + int(Enemies.size()); i < 20; i++) { std::cout << std::endl; }

	std::cout << std::endl << "Press Enter to Continue ";
	std::cin.ignore();

	int combat_status = core_combat(Characters, Enemies, turn_order, battleground); 

	if (combat_status == 0) {
		std::cout << "Victory!" << std::endl;
	}
	else {
		std::cout << "Defeat!" << std::endl;
	}

}

int* roll_initiative(std::vector<Character*> Characters, std::vector<Enemy*> Enemies, int turn_order[]) 
{

	int c_size = Characters.size();
	int e_size = Enemies.size();

	// inititaive rolls for characters
	for (int i = 0; i < c_size; i++) {
		Characters[i]->set_init_roll(rand()%d20);
	}
	// initiative rolls for enemies
	for (int i = 0; i < e_size; i++) {
		Enemies[i]->set_init_roll(rand()%d20);
	}

	int placed[c_size + e_size];
	// set all values in placed to 0 to indicate no one has been placed yet
	for (int i = 0; i < c_size + e_size; i++) {
		placed[i] = 0;
	}
	int curr_max = 0;
	int prev_max = 100;
	int curr_max_init_mod = 0; // used to decide ties
	int curr_max_pos; // used to mark values within placed array

	

	// sorts init_rolls into a turn order (turn order contains indexes of Characters+Enemies)
	// For example, if Characters has a size 1, then index 2 refers to the first element in Enemies
	// i refers to the position in the initiative order, e.g. i = 0 is the slot of the first person to act
	// j refers to the index of a specific combatant
	for (int i = 0; i < c_size + e_size; i++) {
		for (int j = 0; j < c_size + e_size; j++) {
			if (j < c_size && placed[j] == 0) {
				// ties are decided by initiative modifiers (or if those are tied, whoever is visited first)
				if (Characters[j]->get_init_roll() == curr_max) {
					if (Characters[j]->get_init_mod() > curr_max_init_mod) {
						turn_order[i] = j;
						curr_max = Characters[j]->get_init_roll();
						curr_max_init_mod = Characters[j]->get_init_mod();
						curr_max_pos = j;
					}
				}

				// in the case of a tie, puts loser next
				if (Characters[j]->get_init_roll() == prev_max && prev_max && placed[j] == 0) {
					turn_order[i] = j;
					curr_max = Characters[j]->get_init_roll();
					curr_max_init_mod = Characters[j]->get_init_mod();
					curr_max_pos = j;
				}

				if (Characters[j]->get_init_roll() > curr_max && 
					Characters[j]->get_init_roll() < prev_max) {
					turn_order[i] = j;
					curr_max = Characters[j]->get_init_roll();
					curr_max_init_mod = Characters[j]->get_init_mod();
					curr_max_pos = j;
				}
			}
			else if ( placed[j] == 0 ) {
				// ties are decided by initiative modifiers (or if those are tied, whoever is visited first)
				if (Enemies[j - c_size]->get_init_roll() == curr_max) {
					if (Enemies[j - c_size]->get_init_mod() > curr_max_init_mod) {
						turn_order[i] = j;
						curr_max = Enemies[j - c_size]->get_init_roll();
						curr_max_init_mod = Enemies[j - c_size]->get_init_mod();
						curr_max_pos = j;
					}
				}

				// in the case of a tie, puts loser next
				if (Enemies[j - c_size]->get_init_roll() == prev_max && prev_max && placed[j] == 0) {
					turn_order[i] = j;
					curr_max = Enemies[j - c_size]->get_init_roll();
					curr_max_init_mod = Enemies[j - c_size]->get_init_mod();
					curr_max_pos = j;
				}


				if (Enemies[j - c_size]->get_init_roll() > curr_max &&
					Enemies[j - c_size]->get_init_roll() < prev_max) {
					turn_order[i] = j;
					curr_max = Enemies[j - c_size]->get_init_roll();
					curr_max_pos = j;
				}
			}
			else { /* nothing to do */ }
		}
		prev_max = curr_max;
		curr_max = 0;
		placed[curr_max_pos] = 1;
	}

	return turn_order;
}

int core_combat(std::vector<Character*> Characters, std::vector<Enemy*> Enemies, int turn_order[], Map *battleground) 
{

	int characters_dead = 0;
	int enemies_dead = 0;

	// loops until win or loss condition is met
	while (true) { 
		
		for (int i = 0; i < int(Characters.size() + Enemies.size()); i++) {

			if (enemies_dead == int(Enemies.size())) {
				return 0; // victory
			}
			if (characters_dead == int(Characters.size())) {
				return 1; // defeat
			}

			// character turn
			if (turn_order[i] < int(Characters.size())) { 
				if (Characters[turn_order[i]]->get_curr_hp() > 0) {
					enemies_dead += Characters[turn_order[i]]->begin_turn(Enemies, Characters, battleground, 1);
				}
			}
			// enemy turn
			else { 
				if (Enemies[turn_order[i] - Characters.size()]->get_curr_hp() > 0) {
					characters_dead += Enemies[turn_order[i] - Characters.size()]->begin_turn(Characters, Enemies, battleground);
				}
			}
		}
	}
}

