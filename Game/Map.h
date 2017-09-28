// Map.h
// contains information related to the map

#pragma once
#include <vector>

class Map {
public:
	Map();
	Map(int w, int h);
	~Map();
	void draw_map();
	int check_adjacency(int char_index, int enem_index);
	void add_character(int x, int y, char sym);
	void add_enemy(int x, int y, char sym);
	void add_obstacle (int x, int y, char sym);
	char get_map_data(int x, int y)   { return (*map_data)[x][y]; }
	int get_width()  		  		  { return width; }
	int get_height() 		  		  { return height; }
	int get_char_x(int index) 		  { return characters_x[index]; }
	int get_char_y(int index) 		  { return characters_y[index]; }
	void set_char_x(int index, int x) { characters_x[index] = x; }
	void set_char_y(int index, int y) { characters_y[index] = y; }
	int get_enem_x(int index) 		  { return enemies_x[index]; }
	int get_enem_y(int index) 		  { return enemies_y[index]; }
	void set_enem_x(int index, int x) { enemies_x[index] = x; }
	void set_enem_y(int index, int y) { enemies_y[index] = y; }


private:
	int width;
	int height;
	std::vector< std::vector<char> > *map_data;
	std::vector<char> characters_sym;
	std::vector<int>  characters_x;
	std::vector<int>  characters_y;
	std::vector<char> enemies_sym;
	std::vector<int>  enemies_x;
	std::vector<int>  enemies_y;
	std::vector<char> obstacles_sym;
	std::vector<int>  obstacles_x;
	std::vector<int>  obstacles_y;
};