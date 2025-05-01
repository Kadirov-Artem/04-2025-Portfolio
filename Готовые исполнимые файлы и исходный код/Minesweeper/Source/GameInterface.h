#pragma once
#include "Field.h"
#include "GradientRect.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <iomanip>

namespace ms
{

class GameInterface
{
private:
	enum MouseState {NONE, LEFT_BUTTON, RIGHT_BUTTON};

	static const int CELL_SIZE;
	static const int CELL_SHAPE_THICKNESS;
	static const sf::Vector2u REP_WND_SIZE;
	static const float TEXTURE_SIZE;

	Field field;
	sf::RenderWindow *window, *report_window;

	// Drawable objects
	sf::RectangleShape unrevealed_rect_shape, revealed_rect_shape, fatal_rect_shape;
	ms::GradientRect bottom_shape, right_shape;
	sf::Text cell_number_shape, game_report_title, game_report;
	sf::Sprite bomb_sprite, flag_sprite, crossed_flag_sprite;

	// Resources
	sf::Font font;
	sf::SoundBuffer sound_buffer;
	sf::Texture bomb_texture, flag_texture, crossed_flag_texture;

	// Sounds
	sf::Sound explosion_sound;
public:
	/* Constructor */
	GameInterface(int width, int height, int seed, int cells_per_bomb);

	/* Constructor
	   Creates an empty game interface object
	   The "load" function is supposed to be called after creation the object
	   Calling any other functions before the "load" function leads to undefined behaviour
	*/
	GameInterface();

	/* Destructor */
	~GameInterface();

	/* Game loop */
	void main_loop();

	/* Renders the field */
	void render_field();

	/* Handles mouse events */
	void update();

	/* Returns a formatted string with
	   all game statictics
	*/
	std::string get_game_report();

	/* If possible, loads the game from file */
	bool load();
private:
	/* Initializes object's fields */
	void setup();
};

};