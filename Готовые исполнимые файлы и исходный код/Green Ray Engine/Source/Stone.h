#pragma once
#include "Tile.h"

namespace gr
{

class Stone: public Tile
{
protected:
public:
	Stone(int x, int y);  // Constructor

	gr::TileType type() const;  // Returns the tile type id

	// Loads texture and chooses a texture rectangle depending on presence of neighboring tiles
	void updateTexture();

	TileGroup group() const;  // Returns the tile's group
};

}