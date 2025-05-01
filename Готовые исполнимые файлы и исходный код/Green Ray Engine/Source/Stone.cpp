#include "Stone.h"

gr::Stone::Stone(int ax, int ay):
	Tile(ax, ay)
{}

gr::TileType gr::Stone::type() const
{
	return gr::TileType::STONE;
}

void gr::Stone::updateTexture()
{
	sprite.setTexture(*resources.getTexture(gr::Texture::STONE_TEXTURE));

	// Find out, in what directions the tile has neighbors
	char top, right, bottom, left;
	top = container->count({x, y + 1});
	right = container->count({x + 1, y});
	bottom = container->count({x, y - 1});
	left = container->count({x - 1, y});

	// Depending on presence of neighbors, choose corresponging texture rectangle
	if (top + right + bottom + left == 0)  // If there are no neighboring tiles
	{
		setTextureRect(3, 3);
	}
	else if (top + right + bottom + left == 1)  // If there is only one neighboring tile
	{
		if (top)
		{
			setTextureRect(1, 3);
		}
		else if (right)
		{
			setTextureRect(2, 3);
		}
		else if (bottom)
		{
			setTextureRect(3, 2);
		}
		else if (left)
		{
			setTextureRect(0, 3);
		}
	}
	else if (top + right + bottom + left == 2) // If there are two neighboring tiles
	{
		if (top && right)
		{
			setTextureRect(3, 1);
		}
		else if (right && bottom)
		{
			setTextureRect(0, 2);
		}
		else if (bottom && left)
		{
			setTextureRect(1, 1);
		}
		else if (left && top)
		{
			setTextureRect(2, 1);
		}
		else if (top && bottom)
		{
			setTextureRect(2, 2);
		}
		else if (left && right)
		{
			setTextureRect(1, 2);
		}
	}
	else if (top + right + bottom + left == 3) // If there are three neighboring tiles
	{
		if (left && top && right)
		{
			setTextureRect(3, 0);
		}
		else if (top && right && bottom)
		{
			setTextureRect(0, 1);
		}
		else if (right && bottom && left)
		{
			setTextureRect(1, 0);
		}
		else if (bottom && left && top)
		{
			setTextureRect(2, 0);
		}
	}
	else  // If there are four neighboring tiles
	{
		setTextureRect(0, 0);
	}
}

gr::TileGroup gr::Stone::group() const
{
	return gr::DEFAULT;
}