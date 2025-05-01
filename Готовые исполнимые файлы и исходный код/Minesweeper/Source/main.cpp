#include "GameInterface.h"
#include "GreenRayWindow.h"
#include "GreenRayInterface.h"
#include <sstream>

const ms::Coord WND_SIZE(400, 220);
const float FADE_TIME {2000};

int str_to_int(const std::string&);

int main()
{
	gr::Window::hideConsole();  // Спрятать консоль
	//gr::Window::showConsole();  // Показать консоль

	sf::Clock fade_clock;
	float fade_progress {0};

	// Flags
	bool game_running {true};
	int error_occurred {0};
	bool game_loading_initiated {false};
	bool game_loading_error_occurred {false};

	// Load font
	sf::Font font;
	font.loadFromFile("consola.ttf");

	// Create window
	gr::Window window(WND_SIZE.x, WND_SIZE.y, "Minesweeper", sf::Style::Close);

	// Main menu
	gr::Label minesweeper_l(200, 10, 28, "Minesweeper", font, gr::Label::Center);
	gr::Button new_game_b(100, 70, 200, 20, "New game", font);
	gr::Button load_game_b(100, 110, 200, 20, "Load game", font);
	gr::Button settings_b(100, 150, 200, 20, "Settings", font);
	gr::Label version_l(200, 190, 12, "Alpha 1.0.0", font, sf::Color(128, 128, 128), gr::Label::Center);

	// Start menu
	gr::Label new_game_l(200, 10, 28, "New game", font, gr::Label::Center);
	gr::Label field_size_l(10, 70, 20, "Field size", font);
	gr::Label cells_per_bomb_l(10, 100, 20, "Cells per bomb", font);
	gr::Label seed_l(10, 130, 20, "Generator seed", font);
	gr::TextField width_tf(210, 70, 80, 20, "Width", font);
	gr::Label x_l(300, 65, 20, "x", font, gr::Label::Center);
	gr::TextField height_tf(310, 70, 80, 20, "Height", font);
	gr::Slider cells_per_bomb_s(210, 100, 180, 2, 10, 1, font);
	gr::TextField seed_tf(210, 130, 180, 20, "Random value", font);
	gr::Button start_b(20, 170, 170, 20, "Start", font);
	gr::Button start_menu_back_b(210, 170, 170, 20, "Back", font);

	// Load error menu
	gr::Label load_error_title_l(200, 10, 28, "Unable to load the game", font, gr::Label::Center);
	gr::Label load_error_cause_l(200, 70, 20, "There are no unfinished games", font, gr::Label::Center);
	gr::Button load_error_menu_ok_b(100, 170, 200, 20, "Ok", font);

	// Setup the interface
	settings_b.disable();
	cells_per_bomb_s.setValue(6);

	// Game setup data
	int field_width {0};
	int field_height {0};
	int cells_per_bomb {0};
	int seed {0};

	while (game_running)
	{
		// Reset flags
		game_running = false;
		game_loading_initiated = false;
		
		// If game loading failed
		if (game_loading_error_occurred)
		{
			// Display game loading menu error
			window.clearInterface();
			window.clearSpriteBuffer();
			window << load_error_title_l << load_error_cause_l << load_error_menu_ok_b;
			game_loading_error_occurred = false;
		}
		else
		{
			// Return to the main menu
			window.clearInterface();
			window.clearSpriteBuffer();
			window << minesweeper_l << new_game_b << load_game_b << settings_b << version_l;
		}

		// Show the window
		window.show();

		while (window.work())
		{
			// Transition from main menu to start menu
			if (new_game_b.is_triggered())
			{
				window.clearInterface();
				window.clearSpriteBuffer();
				window << new_game_l << field_size_l << cells_per_bomb_l << seed_l << width_tf <<
						  height_tf << x_l << cells_per_bomb_s << seed_tf << start_b << start_menu_back_b;
			}

			// Transition from start menu to main menu
			if (start_menu_back_b.is_triggered())
			{
				window.clearInterface();
				window.clearSpriteBuffer();
				window << minesweeper_l << new_game_b << load_game_b << settings_b << version_l;
			}

			// Transition from loading error menu to main menu
			if (load_error_menu_ok_b.is_triggered())
			{
				window.clearInterface();
				window.clearSpriteBuffer();
				window << minesweeper_l << new_game_b << load_game_b << settings_b << version_l;
			}

			// If the start button is triggered
			if (start_b.is_triggered())
			{
				// Get game setups
				cells_per_bomb = cells_per_bomb_s.getValue();

				game_running = true;

				// Field width
				try
				{
					field_width = str_to_int(width_tf.getText());
					
					if (field_width < 2 || field_width > 100)
					{
						error_occurred = 1;
						game_running = false;
						fade_clock.restart();
					}
				}
				catch (std::string err)
				{
					error_occurred = 1;
					game_running = false;
					fade_clock.restart();
				}

				// Field height
				try
				{
					field_height = str_to_int(height_tf.getText());

					if (field_height < 2 || field_height > 100)
					{
						error_occurred = 2;
						game_running = false;
						fade_clock.restart();
					}
				}
				catch (std::string err)
				{
					error_occurred = 2;
					game_running = false;
					fade_clock.restart();
				}

				// Seed
				try
				{
					if (!seed_tf.getText().getSize())
					{
						std::srand(std::time(NULL));
						seed = std::rand();
					}
					else
					{
						seed = str_to_int(seed_tf.getText());
					}
				}
				catch (std::string err)
				{
					error_occurred = 3;
					game_running = false;
					fade_clock.restart();
				}
			}

			// If the load game button is triggered
			if (load_game_b.is_triggered())
			{
				game_loading_initiated = true;
				game_running = true;
			}

			// Input error fade animation
			if (error_occurred)
			{
				fade_progress = fade_clock.getElapsedTime().asMilliseconds() / FADE_TIME;
				
				// Check for the end of the animation
				if (fade_progress > 1)
				{
					error_occurred = 0;
				}

				// Update the error highlight color
				sf::Color highlight_color(0, 0, 0), highlight_text_color(128, 128, 128);

				if (fade_progress != 0)
				{
					highlight_color.r = (1 - fade_progress) * 256;
					highlight_text_color.r = (1 - fade_progress) * 128 + 128;
				}
				else
				{
					highlight_color.r = 255;
					highlight_text_color.r = 255;
				}
				
				// Change the color of the textfield which contains an error
				switch (error_occurred)
				{
					case 1:
						width_tf.setTextColor(highlight_text_color);
						width_tf.setSelectColor(highlight_color);
						width_tf.setBorderColor(highlight_color);
						break;
					case 2:
						height_tf.setTextColor(highlight_text_color);
						height_tf.setSelectColor(highlight_color);
						height_tf.setBorderColor(highlight_color);
						break;
					case 3:
						seed_tf.setTextColor(highlight_text_color);
						seed_tf.setSelectColor(highlight_color);
						seed_tf.setBorderColor(highlight_color);
						break;
				}
			}

			// Starting the game
			if (game_running)
			{
				window.hide();
				break;
			}
		}
		
		// Check if the game has been started
		if (!game_running)
		{
			break;
		}

		// Initiate the game object
		if (game_loading_initiated)  // If the game loading is initiated
		{
			ms::GameInterface game_interface;
			if (game_interface.load())
			{
				game_interface.main_loop();
			}
			else
			{
				game_loading_error_occurred = true;
			}
		}
		else  // If a new game is started
		{
			ms::GameInterface game_interface(field_width, field_height, seed, cells_per_bomb);
			game_interface.main_loop();
		}
	}

	return 0;
}

int str_to_int(const std::string& arg)
{
	std::istringstream stream;
	stream.str(arg + " ");
	int val;
	stream >> val;
	if (stream)
	{
		return val;
	}
	else
	{
		throw std::string("Invalid input");
	}
}