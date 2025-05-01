#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <sstream>
#include <iomanip>
#include "Texture.h"
#include "Resources.h"
#include "GlobalData.h"
#include "Camera.h"
#include "GameTick.h"
#include "Tile.h"
#include "None.h"
#include "IntCrd.h"

namespace gr
{

class Entity
{
protected:
	int id;  // Unique entity identificator
	float x, y;  // Coordinates of the left bottom corner of the entity hitbox
	float w, h;  // Width and height of the entity hitbox
	float vx, vy;  // Velocity in tiles per second
	sf::Sprite sprite;  // Sprite
	sf::RectangleShape hitbox;  // Entity hitbox
	static int total;  // Total number of entities on the level
	int animation_cycle;  // Number of frames of the animation (width of a tileset)
	int animation_id;  // Number of the animation (row on the tileset)
	bool is_moving_hor, is_moving_ver;  // Moving flags
	std::ostringstream debug_stream;  // Stream for debug information
	static std::map <int, gr::Entity*> *entity_array;  // Pointer to the container of entities
	static std::map <IntCrd, Tile*> *tile_array;  // Pointer to the container of tiles

public:
	Entity(float x, float y, float w, float h);  // Constructor

	~Entity();  // Destructor

	static void render_all(sf::RenderWindow &window);  // Renders all entities at the specified window

	void render(sf::RenderWindow &window);  // Renders the entity at the specified window

	static void update_all();  // Updates all entities

	virtual void update() =0;  // Updates an entity

	virtual void debug();  // Updates debug info

	/* Gives all entities access to the container of entities
	Must be called right after the level generation, before any entity is created */
	static void bind_entity_array(std::map <int, gr::Entity*> *container);

	/* Gives all entities access to the container of tiles
	Must be called right after the level generation, before any entity is created */
	static void bind_tile_array(std::map <IntCrd, Tile*> *container);

	/* Returns a pointer to the specified tile
	If the tile does not exist, returns the None object */
	Tile *getTile(int x, int y);

	inline int get_id() const  // Returns id of the entity
	{return id;}

	std::string get_debug_string() const;  // Returns formatted debug info
};

}