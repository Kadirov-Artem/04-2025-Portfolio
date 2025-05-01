#pragma once
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "IntCrd.h"
#include "Tile.h"
#include "Entity.h"
#include "Player.h"
#include "Stone.h"
#include "Ladder.h"
#include "None.h"
#include "TileType.h"
#include "Debug.h"
#include "Camera.h"
#include "Platform.h"

namespace gr
{

class Level
{
private:
	std::map <IntCrd, Tile*> tile_array;  // Stores tiles of a level
	std::map <int, gr::Entity*> entity_array;  // Stores entities of a level
public:
	/* Loads the specified level from a file
	The file name must match the level name and have a ".grlevel" extension */
	Level(std::string name);  // Constructor

	~Level();  // Destructor

	void render(sf::RenderWindow &wnd);  // Renders all tiles at the specified window

	/* Returns a pointer to the specified tile */
	gr::Tile *getTile(int x, int y);  
};

}