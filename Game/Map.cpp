 // Map.cpp
// Contains functions for drawing and manipulating the map

#include <iostream>
#include "Map.h"
#include <vector>

Map::Map() 
{ 
	width = 40;
	height = 8;

 	map_data = new std::vector< std::vector<char> >(width, std::vector<char>(height));
}

Map::Map(int w, int h) 
{
	width = w;
	height = h;

	map_data = new std::vector< std::vector<char> >(width, std::vector<char>(height));
}

Map::~Map() 
{
	delete map_data;
}


void Map::draw_map() 
{
	for (int i = 0; i < height; i++) {	
		for (int j = 0; j < width; j++) {

			int space_occupied = 0; // 1 indicates space is occupied

			if (i == 0 && j == 0)                       { (*map_data)[j][i] = '+'; }
			else if (i == 0 && j == width - 1) 	        { (*map_data)[j][i] = '+'; }
			else if (i == height - 1 && j == 0)         { (*map_data)[j][i] = '+'; }
			else if (i == height - 1 && j == width - 1) { (*map_data)[j][i] = '+'; }
			else if (j == 0 || j == width - 1)  		{ (*map_data)[j][i] = '|'; }
			else if (i == 0 || i == height - 1) 		{ (*map_data)[j][i] = '-'; }
			else {

				// check for characters
				for (int c = 0; c < int(characters_x.size()); c++) {
					if (characters_x[c] == j && characters_y[c] == i) {
						(*map_data)[j][i] = characters_sym[c];
						space_occupied = 1;
					}
				}

				// check for enemies
				for (int e = 0; e < int(enemies_x.size()); e++) {
					if (enemies_x[e] == j && enemies_y[e] == i) {
						(*map_data)[j][i] = enemies_sym[e];
						space_occupied = 1;
					}
				}

				// check for obstacles
				for (int o = 0; o < int(obstacles_x.size()); o++) {
					if (obstacles_x[o] == j && obstacles_y[o] == i) {
						(*map_data)[j][i] = obstacles_sym[o];
						space_occupied = 1;
					}
				}

				if (space_occupied == 0) {
					(*map_data)[j][i] = ' ';
				}
			}
		}					
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width + 1; j++) {
			if (j == width) { std::cout << std::endl; }
			else { std::cout << (*map_data)[j][i]; }
		}
	}
}

int Map::check_adjacency(int char_index, int enem_index) 
{
	int c_x = characters_x[char_index];
	int c_y = characters_y[char_index];
	int e_x = enemies_x[enem_index];
	int e_y = enemies_y[enem_index];

	if (e_y + 1 == c_y) {
		if (e_x + 1 == c_x or e_x == c_x or e_x - 1 == c_x) {
			return 1; // adjacent
		}
	}
	else if (e_y == c_y) {
		if (e_x + 1 == c_x or e_x - 1 == c_x) {
			return 1; // adjacent
		}
	}
	else if (e_y - 1 == c_y) {
		if (e_x + 1 == c_x or e_x == c_x or e_x - 1 == c_x) {
			return 1; // adjacent
		}
	}
	else { 
		return 0; // not adjacent 
	} 
	return 0;
}

void Map::add_character(int x, int y, char sym) 
{
	characters_x.push_back(x);
	characters_y.push_back(y);
	characters_sym.push_back(sym);
}

void Map::add_enemy(int x, int y, char sym) 
{
	enemies_x.push_back(x);
	enemies_y.push_back(y);
	enemies_sym.push_back(sym);
}
	
void Map::add_obstacle (int x, int y, char sym) 
{
	obstacles_x.push_back(x);
	obstacles_y.push_back(y);
	obstacles_sym.push_back(sym);
}



