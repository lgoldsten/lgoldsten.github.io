#include <iostream>
#include <cstdlib>
#include <vector>
#include "Map.h"

int main() {

	Map map;

	map.add_character(1, 1, 'D');
	map.add_enemy(2, 1, 'I');
	map.add_obstacle(3, 1, 'C');
	map.add_character(4, 1, 'K');
	map.add_obstacle(5, 5, '!');

	map.draw_map();	

	return 0;
}