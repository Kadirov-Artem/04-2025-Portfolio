#include "GlobalData.h"

const int gr::TILE_SIZE = 64;
const int gr::WND_WIDTH = 1280;  
const int gr::WND_HEIGHT = 720;
sf::Font gr::debug_font;
const int gr::DEBUG_FONT_SIZE = 12;

void gr::setup()
{
	debug_font.loadFromFile("lucon.ttf");  // Load font
}