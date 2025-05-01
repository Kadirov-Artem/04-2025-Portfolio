#include "Tile.h"

int gr::Tile::total = 0;
std::map <gr::IntCrd, gr::Tile*> *gr::Tile::container;

gr::Tile::Tile(int ax, int ay):
	x(ax), y(ay), id(total++), tileset_height(0), tileset_width(0)
{}

gr::Tile::~Tile()
{
	--total;
}

void gr::Tile::render(sf::RenderWindow &window)
{
	// Calculate coordinates of the left top corner of the sprite on screen and render it
	// if it is captured by the camera
	int sx, sy;
	if ( cam.crdToPix(x, y, sx, sy) )
	{
		sprite.setPosition(sx, sy);
		window.draw(sprite);
	}
}

void gr::Tile::setTextureRect(int tile_x, int tile_y)
{
	sprite.setTextureRect({tile_x * TILE_SIZE, tile_y * TILE_SIZE, TILE_SIZE, TILE_SIZE});
}

void gr::Tile::bind_container(std::map <gr::IntCrd, gr::Tile*> *acont)
{
	container = acont;
}