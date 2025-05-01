#pragma once
#include "Tile.h"
#include "Texture.h"
#include "Resources.h"

namespace gr
{

class Ladder: public Tile
{
protected:
public:
	Ladder(int x, int y);  // Constructor

	gr::TileType type() const;  // Returns the tile type id

	// Loads the tile's texture
	void updateTexture();

	TileGroup group() const;  // Returns the tile's group
};

}