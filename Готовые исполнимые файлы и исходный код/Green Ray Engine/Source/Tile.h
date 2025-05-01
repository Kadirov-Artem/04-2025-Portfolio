#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <fstream>
#include <string>
#include "TileType.h"
#include "Texture.h"
#include "Resources.h"
#include "GlobalData.h"
#include "Camera.h"
#include "GameTick.h"
#include "IntCrd.h"
#include "TileGroup.h"

namespace gr
{

class Tile
{
protected:
	int id;  // Unique tile identificator
	int x, y;  // Integer tile coordinates (mathes to the left bottom corner coordinate)
	sf::Sprite sprite;  // Sprite
	static int total;  // Total number of tiles on the level
	int tileset_width, tileset_height;  // Width and height of a tileset of a concrete tile type
	static std::map <gr::IntCrd, gr::Tile*> *container;  // Pointer to the container of tiles
public:
	Tile(int x, int y);  // Constructor

	~Tile();  // Destructor

	void render(sf::RenderWindow &window);  // Renders the tile at the specified window

	virtual gr::TileType type() const =0;

	virtual void updateTexture() =0;  // Updates the tile's texture depending on certain factors

	/* Sets texture rectangle
	tile_x: x-coordinate of a 64x64 area on the texture png
	tile_x: y-coordinate of a 64x64 area on the texture png */
	void setTextureRect(int tile_x, int tile_y);

	virtual TileGroup group() const =0;  // Returns the tile's group

	/* Gives all tile access to the container of tiles
	Must be called right after the level generation, before any tile is created */
	static void bind_container(std::map <gr::IntCrd, gr::Tile*> *container);
};

}