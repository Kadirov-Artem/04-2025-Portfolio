#include "Level.h"

gr::Level::Level(std::string name)
{
	// Bind containers to tiles and entities
	gr::Tile::bind_container(&tile_array);
	gr::Entity::bind_entity_array(&entity_array);
	gr::Entity::bind_tile_array(&tile_array);

	// Load all tiles from a file
	std::ifstream stream(name + ".grlevel");
	if(stream)
	{
		int world_width, world_height, player_x, player_y;
		char t;
		stream >> world_width >> world_height >> player_x >> player_y;

		// Relocate the camera
		cam.moveTo(player_x, player_y);

		// Create a player
		gr::Entity *ptr_player = new gr::Player(player_x, player_y);
		entity_array.emplace(ptr_player->get_id(), ptr_player);

		// For test
		//gr::Entity *ptr_platform = new gr::Platform(12, 10, std::vector<FltCrd> {});
		//entity_array.emplace(ptr_platform->get_id(), ptr_platform);

		// Bind the player to the debugger
		debug.bind_tracked_entity(ptr_player);

		for (int i = 0; i < world_height; i++)
		{
			for (int j = 0; j < world_width; j++)
			{
				stream >> t;
				int x = j;
				int y = world_height - i - 1;

				switch (t)
				{
				case '#':
					tile_array.emplace(gr::IntCrd(x, y), new gr::Stone(x, y));
					break;
				case '$':
					tile_array.emplace(gr::IntCrd(x, y), new gr::Ladder(x, y));
					break;
				default:
					break;
				}
			}
		}
	}
	else
	{
		throw std::string("gr::Level::Level: unable to open the level file");
	}

	// Update textures of all tiles
	for (std::map <IntCrd, Tile*>::iterator it = tile_array.begin();
		 it != tile_array.end(); it++)
	{
		it->second->updateTexture();
	}
}

gr::Level::~Level()
{
	for (std::map <IntCrd, Tile*>::iterator it = tile_array.begin();
		 it != tile_array.end(); it++)
	{
		delete it->second;
	}
}

void gr::Level::render(sf::RenderWindow &wnd)
{
	for (std::map <IntCrd, Tile*>::iterator it = tile_array.begin();
		 it != tile_array.end(); it++)
	{
		it->second->render(wnd);
	}
}

gr::Tile *gr::Level::getTile(int ax, int ay)
{
	if ( tile_array.count(IntCrd(ax, ay)) )
	{
		return tile_array.at(IntCrd(ax, ay));
	}
	else
	{
		return &empty_tile;
	}
}