#include "Debug.h"

gr::Debug gr::debug;

gr::Debug::Debug():
	fps(0), fps_measured(false), tracked_entity(NULL)
{
	text.setCharacterSize(gr::DEBUG_FONT_SIZE);
	text.setFont(gr::debug_font);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1);
	text.setPosition(0, 0);
}

void gr::Debug::update()
{
	stream.str("");  // Clear debug stream

	// Reset fps_measure flag
	if (game_tick.get_tick() % 20 == 1)
	{
		fps_measured = false;
	}

	// Update fps value
	if (!fps_measured && game_tick.get_tick() % 20 == 0)
	{
		fps_measured = true;
		fps = int(1 / game_tick.get_period());  // Measure fps
	}

	stream << "fps = " << fps << std::endl;  // Append information about fps

	tracked_entity->debug();  // Update the player's debug info
	stream << tracked_entity->get_debug_string();  // Append the player's debug information
}

void gr::Debug::print(sf::RenderWindow &window)
{
	text.setString(stream.str());  // Set debug string
	window.draw(text);  // Render debug info
}

void gr::Debug::bind_tracked_entity(Entity *aentity)
{
	tracked_entity = aentity;
}