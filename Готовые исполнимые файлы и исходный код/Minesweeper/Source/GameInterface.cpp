#include "GameInterface.h"

const int ms::GameInterface::CELL_SIZE = 20;
const int ms::GameInterface::CELL_SHAPE_THICKNESS = 5;
const sf::Vector2u ms::GameInterface::REP_WND_SIZE(400, 300);
const float ms::GameInterface::TEXTURE_SIZE = 50;

ms::GameInterface::GameInterface(int w, int h, int s, int cpb):
	field(w, h, s, cpb),
	window(NULL),
	report_window(NULL),
	bottom_shape(CELL_SIZE, CELL_SHAPE_THICKNESS, sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0),
				 sf::Color(0, 0, 0, 127), sf::Color(0, 0, 0, 127)),
	right_shape(CELL_SHAPE_THICKNESS, CELL_SIZE, sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 127),
				 sf::Color(0, 0, 0, 127), sf::Color(0, 0, 0, 0))
{
	window = new sf::RenderWindow(sf::VideoMode(CELL_SIZE * w, CELL_SIZE * h), "Minesweeper", sf::Style::Close);
	report_window = new sf::RenderWindow(sf::VideoMode(REP_WND_SIZE.x, REP_WND_SIZE.y), "Report", sf::Style::Close);
	setup();
}

ms::GameInterface::GameInterface():
	window(NULL),
	report_window(NULL),
	bottom_shape(CELL_SIZE, CELL_SHAPE_THICKNESS, sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0),
				 sf::Color(0, 0, 0, 127), sf::Color(0, 0, 0, 127)),
	right_shape(CELL_SHAPE_THICKNESS, CELL_SIZE, sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 127),
				sf::Color(0, 0, 0, 127), sf::Color(0, 0, 0, 0))
{
	setup();
}

ms::GameInterface::~GameInterface()
{
	if (window)  // Destroy windows
	{
		delete window;  
	}

	if (report_window)
	{
		delete report_window;
	}
}

void ms::GameInterface::setup()
{
	// Load resources
	sound_buffer.loadFromFile("explosion.mp3");
	font.loadFromFile("consola.ttf");
	explosion_sound.setBuffer(sound_buffer);
	bomb_texture.loadFromFile("bomb.png");
	flag_texture.loadFromFile("flag.png");
	crossed_flag_texture.loadFromFile("crossed_flag.png");

	// Setup drawable objects for rendering
	right_shape.setOrigin(-CELL_SIZE + CELL_SHAPE_THICKNESS, 0);
	bottom_shape.setOrigin(0, -CELL_SIZE + CELL_SHAPE_THICKNESS);

	unrevealed_rect_shape.setOutlineThickness(0);
	unrevealed_rect_shape.setFillColor(sf::Color(150, 150, 150));
	unrevealed_rect_shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));

	revealed_rect_shape.setOutlineThickness(0);
	revealed_rect_shape.setFillColor(sf::Color(200, 200, 200));
	revealed_rect_shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));

	fatal_rect_shape.setOutlineThickness(0);
	fatal_rect_shape.setFillColor(sf::Color(255, 0, 0));
	fatal_rect_shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));

	cell_number_shape.setOutlineThickness(0);
	cell_number_shape.setCharacterSize(CELL_SIZE);
	cell_number_shape.setFont(font);
	cell_number_shape.setOrigin(sf::Vector2f(-CELL_SIZE / 5, CELL_SIZE / 4));

	bomb_sprite.setTexture(bomb_texture);
	bomb_sprite.setScale(CELL_SIZE / TEXTURE_SIZE,
						 CELL_SIZE / TEXTURE_SIZE);

	flag_sprite.setTexture(flag_texture);
	flag_sprite.setScale(CELL_SIZE / TEXTURE_SIZE,
						 CELL_SIZE / TEXTURE_SIZE);

	crossed_flag_sprite.setTexture(crossed_flag_texture);
	crossed_flag_sprite.setScale(CELL_SIZE / TEXTURE_SIZE,
								 CELL_SIZE / TEXTURE_SIZE);

	// Setup text objects
	game_report_title.setFont(font);
	game_report_title.setCharacterSize(28);
	game_report_title.setFillColor(sf::Color::Black);

	game_report.setFont(font);
	game_report.setCharacterSize(20);
	game_report.setFillColor(sf::Color::Black);
	game_report.setPosition(REP_WND_SIZE.x * 0.125, REP_WND_SIZE.y * 0.2);
}

void ms::GameInterface::main_loop()
{
	// Initialize flags
	bool game_ended = false;

	// Hide report window
	report_window->setVisible(false);

	while (window->isOpen() && report_window->isOpen())
	{
		sf::Event event;

		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
		}

		while (report_window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				report_window->close();
			}
		}

		report_window->clear(sf::Color::White);
		report_window->draw(game_report_title);
		report_window->draw(game_report);
		report_window->display();

		update();
		window->clear();
		render_field();
		window->display();

		if (field.is_game_over())
		{
			if (!game_ended)
			{
				// Delete the game save file
				field.delete_save();

				if (!field.is_victory())
				{
					// Play explosion sound
					explosion_sound.play();

					// Setup the title
					game_report_title.setString("Defeat");
					game_report_title.setPosition(REP_WND_SIZE.x / 2 - game_report_title.getGlobalBounds().width / 2, 15);
				}
				else
				{
					// Setup the title
					game_report_title.setString("Victory");
					game_report_title.setPosition(REP_WND_SIZE.x / 2 - game_report_title.getGlobalBounds().width / 2, 15);
				}

				// Get game report
				game_report.setString(get_game_report());

				// Show report window
				report_window->setVisible(true);

				game_ended = true;
			}
		}
	}

	if (!game_ended)
	{
		field.save();
	}
}

void ms::GameInterface::render_field()
{
	// Render cells of the field

	for (int y = 0; y < field.size().y; y++)
	{
		for (int x = 0; x < field.size().x; x++)
		{
			Field::State cell_state = field.get_cell(x, y);  // Get cell state

			// Draw cell background

			if (cell_state == Field::UNREVEALED)
			{
				unrevealed_rect_shape.setPosition(sf::Vector2f(CELL_SIZE * x, CELL_SIZE * y));
				window->draw(unrevealed_rect_shape);
			}
			else if (cell_state == Field::BOMB)
			{
				unrevealed_rect_shape.setPosition(sf::Vector2f(CELL_SIZE * x, CELL_SIZE * y));
				window->draw(unrevealed_rect_shape);

				bomb_sprite.setPosition(sf::Vector2f(CELL_SIZE * x, CELL_SIZE * y));
				window->draw(bomb_sprite);
			}
			else if (cell_state == Field::FATAL)
			{
				fatal_rect_shape.setPosition(sf::Vector2f(CELL_SIZE * x, CELL_SIZE * y));
				window->draw(fatal_rect_shape);

				bomb_sprite.setPosition(sf::Vector2f(CELL_SIZE * x, CELL_SIZE * y));
				window->draw(bomb_sprite);
			}
			else if (cell_state == Field::TAGGED)
			{
				revealed_rect_shape.setPosition(sf::Vector2f(CELL_SIZE * x, CELL_SIZE * y));
				window->draw(revealed_rect_shape);

				flag_sprite.setPosition(sf::Vector2f(CELL_SIZE * x, CELL_SIZE * y));
				window->draw(flag_sprite);
			}
			else if (cell_state == Field::INCORRECTLY_TAGGED)
			{
				revealed_rect_shape.setPosition(sf::Vector2f(CELL_SIZE * x, CELL_SIZE * y));
				window->draw(revealed_rect_shape);

				crossed_flag_sprite.setPosition(sf::Vector2f(CELL_SIZE * x, CELL_SIZE * y));
				window->draw(crossed_flag_sprite);
			}
			else
			{
				revealed_rect_shape.setPosition(sf::Vector2f(CELL_SIZE * x, CELL_SIZE * y));
				window->draw(revealed_rect_shape);

				// Draw cell content
				cell_number_shape.setPosition(sf::Vector2f(CELL_SIZE * x, CELL_SIZE * y));
				
				switch (cell_state)
				{
					case Field::ZERO:
						cell_number_shape.setString("");
						break;
					case Field::ONE:
						cell_number_shape.setString("1");
						cell_number_shape.setFillColor(sf::Color(0, 0, 255));
						break;
					case Field::TWO:
						cell_number_shape.setString("2");
						cell_number_shape.setFillColor(sf::Color(0, 127, 0));
						break;
					case Field::THREE:
						cell_number_shape.setString("3");
						cell_number_shape.setFillColor(sf::Color(255, 0, 0));
						break;
					case Field::FOUR:
						cell_number_shape.setString("4");
						cell_number_shape.setFillColor(sf::Color(0, 127, 127));
						break;
					case Field::FIVE:
						cell_number_shape.setString("5");
						cell_number_shape.setFillColor(sf::Color(255, 0, 255));
						break;
					case Field::SIX:
						cell_number_shape.setString("6");
						cell_number_shape.setFillColor(sf::Color(127, 127, 0));
						break;
					case Field::SEVEN:
						cell_number_shape.setString("7");
						cell_number_shape.setFillColor(sf::Color(255, 127, 0));
						break;
					case Field::EIGHT:
						cell_number_shape.setString("8");
						cell_number_shape.setFillColor(sf::Color(127, 0, 255));
						break;
				}

				window->draw(cell_number_shape);
			}

			// Draw cell borders

			right_shape.setPosition(CELL_SIZE * x, CELL_SIZE * y);
			right_shape.draw(*window);
			bottom_shape.setPosition(CELL_SIZE * x, CELL_SIZE * y);
			bottom_shape.draw(*window);
		}
	}
}

void ms::GameInterface::update()
{
	static MouseState mouse_state = NONE;
	static ms::Coord cursor_pos(0, 0);
	static int cell_x, cell_y;

	if (!window->hasFocus())
	{
		return;
	}

	if (mouse_state == NONE)  // If none of mouse buttons are pressed
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			mouse_state = LEFT_BUTTON;
			cursor_pos.x = sf::Mouse::getPosition(*window).x;  // Get cursor coorinates
			cursor_pos.y = sf::Mouse::getPosition(*window).y;
			cell_x = cursor_pos.x / CELL_SIZE;  // Calculate the cell coordinates
			cell_y = cursor_pos.y / CELL_SIZE;
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			mouse_state = RIGHT_BUTTON;
			cursor_pos.x = sf::Mouse::getPosition(*window).x;  // Get cursor coorinates
			cursor_pos.y = sf::Mouse::getPosition(*window).y;
			cell_x = cursor_pos.x / CELL_SIZE;  // Calculate the cell coordinates
			cell_y = cursor_pos.y / CELL_SIZE;
		}
	}
	else if (mouse_state == LEFT_BUTTON)  // If left button is pressed
	{
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))  // If left button is released
		{
			mouse_state = NONE;
			cursor_pos.x = sf::Mouse::getPosition(*window).x;  // Get cursor coorinates
			cursor_pos.y = sf::Mouse::getPosition(*window).y;
			int cell_x_2 = cursor_pos.x / CELL_SIZE;  // Calculate the cell coordinates again
			int cell_y_2 = cursor_pos.y / CELL_SIZE;

			if (cell_x_2 == cell_x && cell_y_2 == cell_y)  // If this is the same cell
			{
				field.dig(cell_x, cell_y);  // Dig
			}
		}
	}
	else  // If right button is pressed
	{
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Right))  // If right button is released
		{
			mouse_state = NONE;
			cursor_pos.x = sf::Mouse::getPosition(*window).x;  // Get cursor coorinates
			cursor_pos.y = sf::Mouse::getPosition(*window).y;
			int cell_x_2 = cursor_pos.x / CELL_SIZE;  // Calculate the cell coordinates again
			int cell_y_2 = cursor_pos.y / CELL_SIZE;

			if (cell_x_2 == cell_x && cell_y_2 == cell_y)  // If this is the same cell
			{
				field.tag(cell_x, cell_y);  // Tag
			}
		}
	}
}

std::string ms::GameInterface::get_game_report()
{
	std::ostringstream stream;
	stream << "Time: " << field.get_time() << " s" << std::endl;
	stream << "Cells revealed: " << field.get_revealed_cells_count() << '/' <<
		      field.get_size().y * field.get_size().x - field.get_bombs_count() << std::endl;
	stream << "Bombs found: " << field.get_found_bombs_count() << '/' <<
			  field.get_bombs_count() << std::endl;
	stream << "The first cell: (" << field.get_first_cell().x << ", " <<
			  field.get_first_cell().y << ")" << std::endl;
	stream << "Cells per bomb: " << field.get_cells_per_bomb() << std::endl;
	stream << "Field size: " << field.get_size().x << " x " << field.get_size().y << std::endl;
	stream << "Generator seed: " << field.get_seed() << std::endl;

	return stream.str();
}

bool ms::GameInterface::load()
{
	// Try to load the game
	if (!field.load())
	{
		return false;
	}

	// Create the windows
	window = new sf::RenderWindow(sf::VideoMode(CELL_SIZE * field.size().x,
								                CELL_SIZE * field.size().y),
								  "Minesweeper", sf::Style::Close);

	report_window = new sf::RenderWindow(sf::VideoMode(REP_WND_SIZE.x, REP_WND_SIZE.y), "Report", sf::Style::Close);
	return true;
}