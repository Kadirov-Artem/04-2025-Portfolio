#include "Ladder.h"

gr::Ladder::Ladder(int ax, int ay):
	Tile(ax, ay)
{}

gr::TileType gr::Ladder::type() const
{
	return gr::TileType::LADDER;
}

void gr::Ladder::updateTexture()
{
	sprite.setTexture(*resources.getTexture(gr::Texture::LADDER_TEXTURE));
}

gr::TileGroup gr::Ladder::group() const
{
	return gr::CLIMBABLE;
}