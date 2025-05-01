#pragma once
#include "Tile.h"
#include "TileType.h"

namespace gr
{

class None: public Tile
{
protected:
public:
	None(int x, int y);  // Constructor

	gr::TileType type() const;  // Returns the tile type id

	// Does nothing for this type of tile
	void updateTexture();

	TileGroup group() const;  // Returns the tile's group
};

extern gr::None empty_tile;  // Predefined object, for technical purposes

}