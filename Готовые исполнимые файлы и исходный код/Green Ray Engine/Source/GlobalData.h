#pragma once
#include <SFML/Graphics.hpp>

namespace gr
{

extern const int TILE_SIZE;  // Tile size in pixels
extern const int WND_WIDTH;  // Window width in pixels
extern const int WND_HEIGHT;  // Window height in pixels
extern sf::Font debug_font;  // Font for printing debug info
extern const int DEBUG_FONT_SIZE;

void setup();  // Performes a one-time setup for global objects
			   // Should only be called once

}