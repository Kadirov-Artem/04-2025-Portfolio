#include "None.h"

gr::None gr::empty_tile(0, 0);

gr::None::None(int ax, int ay):
	Tile(ax, ay)
{}

gr::TileType gr::None::type() const
{
	return gr::TileType::NONE;
}

gr::TileGroup gr::None::group() const
{
	return gr::PENETRATABLE;
}

void gr::None::updateTexture()
{}