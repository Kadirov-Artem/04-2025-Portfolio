#include "Field.h"

// struct Coord

bool ms::Coord::operator ==(const Coord &right) const
{
	return x == right.x && y == right.y;
}

// class Field

const std::string ms::Field::SAVE_FILENAME = "save.dat";

ms::Field::Field(int w, int h, int seed, int cpb):
	game_over(false), victory(false), total_bombs(0), WIDTH(w), HEIGHT(h), generated(false),
	CPB(cpb), SEED(seed), first_cell(0, 0), fatal_cell(0, 0), start_time(0)
{
	// Check for exceptions

	if (w < 2 || h < 2)  // If the field is too small
	{
		throw std::string("The field is too small\nWidth and height of the field must be >= 2");
	}

	if (cpb < 2)  // If there are too few cells per bomb
	{
		throw std::string("There are too few cells per bomb\nParameter \"cpb\" must be >= 2");
	}
}

ms::Field::Field():
	game_over(false), victory(false), total_bombs(0), generated(false), first_cell(0, 0),
	fatal_cell(0, 0), CPB(0), SEED(0), start_time(0), WIDTH(0), HEIGHT(0)
{}

void ms::Field::generate(int x, int y)
{
	if (generated)
	{
		throw std::string("The field has already been generated");
	}

	if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
	{
		return;
	}

	generated = true;

	first_cell.x = x;
	first_cell.y = y;

	std::srand(SEED);  // Inititate pseudorandom generator with a seed
	total_bombs = 0;  // Reset the bomb counter

	for (int i = 0; i < HEIGHT; i++)  // Place bombs
	{
		std::vector <bool> row;
		bombs_data.push_back(row);
		for (int j = 0; j < WIDTH; j++)
		{
			if (std::rand() % CPB == 0 && (i != y || j != x))
			{
				bombs_data.back().push_back(1);  // Place a bomb into the cell
				total_bombs++;
			}
			else
			{
				bombs_data.back().push_back(0);  // Tag the cell as free
			}
		}
	}

	for (int i = 0; i < HEIGHT; i++)  // Prepare a zero-matrix
	{
		std::vector <char> row;
		field_data.push_back(row);
		for (int j = 0; j < WIDTH; j++)
		{
			field_data.back().push_back(0);
		}
	}

	// Filling corner cells

	if (bombs_data[0][0] == 0)  // If the top left cell is free
	{
		field_data[0][0] += bombs_data[0][1];  // Count neighboring bombs
		field_data[0][0] += bombs_data[1][0];
		field_data[0][0] += bombs_data[1][1];
	}
	else
	{
		field_data[0][0] = -1;  // Otherwise, tag the cell as containing a bomb
	}
	if (bombs_data[0][WIDTH - 1] == 0)  // If the top right cell is free
	{
		field_data[0][WIDTH - 1] += bombs_data[0][WIDTH - 2]; // Count neighboring bombs
		field_data[0][WIDTH - 1] += bombs_data[1][WIDTH - 1];
		field_data[0][WIDTH - 1] += bombs_data[1][WIDTH - 2];
	}
	else
	{
		field_data[0][WIDTH - 1] = -1; // Otherwise, tag the cell as containing a bomb
	}
	if (bombs_data[HEIGHT - 1][0] == 0)  // If the bottom left cell is free
	{
		field_data[HEIGHT - 1][0] += bombs_data[HEIGHT - 2][0]; // Count neighboring bombs
		field_data[HEIGHT - 1][0] += bombs_data[HEIGHT - 1][1];
		field_data[HEIGHT - 1][0] += bombs_data[HEIGHT - 2][1];
	}
	else
	{
		field_data[HEIGHT - 1][0] = -1; // Otherwise, tag the cell as containing a bomb
	}
	if (bombs_data[HEIGHT - 1][WIDTH - 1] == 0)  // If the bottom right cell is free
	{
		field_data[HEIGHT - 1][WIDTH - 1] += bombs_data[HEIGHT - 1][WIDTH - 2]; // Count neighboring bombs
		field_data[HEIGHT - 1][WIDTH - 1] += bombs_data[HEIGHT - 2][WIDTH - 1];
		field_data[HEIGHT - 1][WIDTH - 1] += bombs_data[HEIGHT - 2][WIDTH - 2];
	}
	else
	{
		field_data[HEIGHT - 1][WIDTH - 1] = -1; // Otherwise, tag the cell as containing a bomb
	}

	// Filling edge non-corner cells

	for (int j = 1; j < WIDTH - 1; j++)  // Fill the top edge
	{
		if (bombs_data[0][j] == 0)  // If the cell is free
		{
			field_data[0][j] += bombs_data[0][j - 1];
			field_data[0][j] += bombs_data[0][j + 1];
			field_data[0][j] += bombs_data[1][j - 1];
			field_data[0][j] += bombs_data[1][j];
			field_data[0][j] += bombs_data[1][j + 1];
		}
		else
		{
			field_data[0][j] = -1;
		}
	}
	for (int i = 1; i < HEIGHT - 1; i++)  // Fill the right edge
	{
		if (bombs_data[i][WIDTH - 1] == 0)  // If the cell is free
		{
			field_data[i][WIDTH - 1] += bombs_data[i - 1][WIDTH - 1];
			field_data[i][WIDTH - 1] += bombs_data[i + 1][WIDTH - 1];
			field_data[i][WIDTH - 1] += bombs_data[i - 1][WIDTH - 2];
			field_data[i][WIDTH - 1] += bombs_data[i][WIDTH - 2];
			field_data[i][WIDTH - 1] += bombs_data[i + 1][WIDTH - 2];
		}
		else
		{
			field_data[i][WIDTH - 1] = -1;
		}
	}
	for (int j = 1; j < WIDTH - 1; j++)  // Fill the bottom edge
	{
		if (bombs_data[HEIGHT - 1][j] == 0)  // If the cell is free
		{
			field_data[HEIGHT - 1][j] += bombs_data[HEIGHT - 1][j - 1];
			field_data[HEIGHT - 1][j] += bombs_data[HEIGHT - 1][j + 1];
			field_data[HEIGHT - 1][j] += bombs_data[HEIGHT - 2][j - 1];
			field_data[HEIGHT - 1][j] += bombs_data[HEIGHT - 2][j];
			field_data[HEIGHT - 1][j] += bombs_data[HEIGHT - 2][j + 1];
		}
		else
		{
			field_data[HEIGHT - 1][j] = -1;
		}
	}
	for (int i = 1; i < HEIGHT - 1; i++)  // Fill the left edge
	{
		if (bombs_data[i][0] == 0)  // If the cell is free
		{
			field_data[i][0] += bombs_data[i - 1][0];
			field_data[i][0] += bombs_data[i + 1][0];
			field_data[i][0] += bombs_data[i - 1][1];
			field_data[i][0] += bombs_data[i][1];
			field_data[i][0] += bombs_data[i + 1][1];
		}
		else
		{
			field_data[i][0] = -1;
		}
	}

	// Filling non-edge cells
	for (int i = 1; i < HEIGHT - 1; i++)
	{
		for (int j = 1; j < WIDTH - 1; j++)
		{
			if (bombs_data[i][j] == 0)  // If the cell is free
			{
				field_data[i][j] += bombs_data[i - 1][j - 1];
				field_data[i][j] += bombs_data[i - 1][j];
				field_data[i][j] += bombs_data[i - 1][j + 1];
				field_data[i][j] += bombs_data[i][j - 1];
				field_data[i][j] += bombs_data[i][j + 1];
				field_data[i][j] += bombs_data[i + 1][j - 1];
				field_data[i][j] += bombs_data[i + 1][j];
				field_data[i][j] += bombs_data[i + 1][j + 1];
			}
			else
			{
				field_data[i][j] = -1;
			}
			}
		}

	// Restart clock
	clock.restart();

	// Debug

#ifdef MINESWEEPER_DEBUG
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			std::cout << std::setw(3) << int(field_data[i][j]);
		}
		std::cout << std::endl;
	}
#endif
}

void ms::Field::dig(int x, int y, bool save_to_buffer)
{
	// Check for exceptions
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
	{
		return;
	}

	// Check if the field has been generated
	if (!generated)
	{
		generate(x, y);
	}

	// If the cell has already been discovered or tagged or the game is over
	if (std::find(revealed_cells.begin(), revealed_cells.end(), Coord(x, y)) != revealed_cells.end() ||
		std::find(tagged_cells.begin(), tagged_cells.end(), Coord(x, y)) != tagged_cells.end() ||
		game_over)
	{
		return;
	}
	
	if (save_to_buffer)
	{
		click_buffer.push_back(Coord(x, y));  // Save the coordinates into the click buffer
	}

	if (bombs_data[y][x])  // If a bomb is come across
	{
		revealed_cells.push_back(Coord(x, y));
		game_over = true;
		fatal_cell.x = x;
		fatal_cell.y = y;
	}
	else if (field_data[y][x] != 0) // If there are bombs near the dug cell
	{
		revealed_cells.push_back(Coord(x, y));  // Just reveal and do nothing else
	}
	else
	{
		// reveal all neighboring empty cells that have no neighboring bombs

		std::vector <Coord> revealed_1, revealed_2, revealed_3, all_revealed;  // Lists of cells revealed on different steps
		revealed_1.push_back(Coord(x, y));
		revealed_2.push_back(Coord(x, y));
		revealed_3.push_back(Coord(x, y));
		all_revealed.push_back(Coord(x, y));
		do
		{
			// Move the lists and clear the third one
			revealed_1 = revealed_2;
			revealed_2 = revealed_3;
			revealed_3.clear();

			// For each cell from "revealed_2" find zero-neighbors which are not contained by
			// "revealed_2", "revealed_1", and "revealed_3"

			std::vector <Coord>::iterator iter;
			for (iter = revealed_2.begin(); iter != revealed_2.end(); iter++)
			{
				// Get the list of neighboring cells
				std::vector <Coord> neighboring = find_neighboring(iter->x, iter->y);

				// Remove non-zero cells and cells which are contained by any "revealed" list
				neighboring.erase(std::remove_if(neighboring.begin(), neighboring.end(), [this](const Coord &crd){return field_data[crd.y][crd.x] != 0;}), neighboring.end());
				neighboring.erase(std::remove_if(neighboring.begin(), neighboring.end(), [revealed_1](const Coord &crd){return std::find(revealed_1.begin(), revealed_1.end(), crd) != revealed_1.end();}), neighboring.end());
				neighboring.erase(std::remove_if(neighboring.begin(), neighboring.end(), [revealed_2](const Coord &crd){return std::find(revealed_2.begin(), revealed_2.end(), crd) != revealed_2.end();}), neighboring.end());
				neighboring.erase(std::remove_if(neighboring.begin(), neighboring.end(), [revealed_3](const Coord &crd){return std::find(revealed_3.begin(), revealed_3.end(), crd) != revealed_3.end();}), neighboring.end());

				// Add the found cells to the "revealed_3" list
				std::vector <Coord>::iterator iter2;
				for (iter2 = neighboring.begin(); iter2 != neighboring.end(); iter2++)
				{
					revealed_3.push_back(*iter2);
				}
			}

			// Save all newly found cells into the "all_revealed" list
			for (iter = revealed_3.begin(); iter != revealed_3.end(); iter++)
			{
				all_revealed.push_back(*iter);
			}
		} while(revealed_3.size());

		// Now we have all zero cells which are surrounded by positive-number cells
		// For each of these cells find neighboring ones, which are not zeros or bombs
		std::vector <Coord>::iterator iter;
		
		for (iter = all_revealed.begin(); iter != all_revealed.end(); iter++)
		{
			// Get the list of neighboring cells
			std::vector <Coord> neighboring = find_neighboring(iter->x, iter->y);

			// Remove zero and bomb cells, those which have already been revealed before, and those which have already been found and saved to "revealed_3" list
			neighboring.erase(std::remove_if(neighboring.begin(), neighboring.end(), [this](const Coord &crd){return field_data[crd.y][crd.x] <= 0;}), neighboring.end());
			neighboring.erase(std::remove_if(neighboring.begin(), neighboring.end(), [revealed_3](const Coord &crd){return std::find(revealed_3.begin(), revealed_3.end(), crd) != revealed_3.end();}), neighboring.end());
			neighboring.erase(std::remove_if(neighboring.begin(), neighboring.end(), [this](const Coord &crd){return std::find(revealed_cells.begin(), revealed_cells.end(), crd) != revealed_cells.end();}), neighboring.end());

			// Add the found cells to the "revealed_3" list (it is empty on the first iteration)
			std::vector <Coord>::iterator iter2;
			for (iter2 = neighboring.begin(); iter2 != neighboring.end(); iter2++)
			{
				revealed_3.push_back(*iter2);
			}
		}

		// Copy all found surrounding non-zero and non-bomb cells into the "all_revealed"
		for (iter = revealed_3.begin(); iter != revealed_3.end(); iter++)
		{
			all_revealed.push_back(*iter);
		}

		// If some of the revealed cells have already been tagged, remove the tags
		tagged_cells.erase(std::remove_if(tagged_cells.begin(), tagged_cells.end(), [all_revealed](const Coord &crd){return std::find(all_revealed.begin(), all_revealed.end(), crd) != all_revealed.end();}), tagged_cells.end());

		// Copy all found cells into "revealed_cells"
		for (iter = all_revealed.begin(); iter != all_revealed.end(); iter++)
		{
			revealed_cells.push_back(*iter);
		}
	}

	// Debug
#ifdef MINESWEEPER_DEBUG
	std::cout << "Revealed cells: " << revealed_cells.size() << std::endl;
#endif
	// Check for victory
	check_victory();
}

std::vector <ms::Coord> ms::Field::find_neighboring(int x, int y) const
{
	// Check for exceptions
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
	{
		throw(std::string("ms::Field::find_neighboring(): the specified coordinate is out of the border of the field"));
	}

	std::vector <Coord> result;

	if (x == 0)
	{
		if (y == 0)
		{
			result.push_back(Coord(x + 1, y));
			result.push_back(Coord(x, y + 1));
			result.push_back(Coord(x + 1, y + 1));
		}
		else if (y == HEIGHT - 1)
		{
			result.push_back(Coord(x, y - 1));
			result.push_back(Coord(x + 1, y - 1));
			result.push_back(Coord(x + 1, y));
		}
		else
		{
			result.push_back(Coord(x, y - 1));
			result.push_back(Coord(x + 1, y - 1));
			result.push_back(Coord(x + 1, y));
			result.push_back(Coord(x, y + 1));
			result.push_back(Coord(x + 1, y + 1));
		}
	}
	else if (x == WIDTH - 1)
	{
		if (y == 0)
		{
			result.push_back(Coord(x - 1, y));
			result.push_back(Coord(x - 1, y + 1));
			result.push_back(Coord(x, y + 1));
		}
		else if (y == HEIGHT - 1)
		{
			result.push_back(Coord(x - 1, y - 1));
			result.push_back(Coord(x, y - 1));
			result.push_back(Coord(x - 1, y));
		}
		else
		{
			result.push_back(Coord(x - 1, y - 1));
			result.push_back(Coord(x, y - 1));
			result.push_back(Coord(x - 1, y));
			result.push_back(Coord(x - 1, y + 1));
			result.push_back(Coord(x, y + 1));
		}
	}
	else
	{
		if (y == 0)
		{
			result.push_back(Coord(x - 1, y));
			result.push_back(Coord(x + 1, y));
			result.push_back(Coord(x - 1, y + 1));
			result.push_back(Coord(x, y + 1));
			result.push_back(Coord(x + 1, y + 1));
		}
		else if (y == HEIGHT - 1)
		{
			result.push_back(Coord(x - 1, y - 1));
			result.push_back(Coord(x, y - 1));
			result.push_back(Coord(x + 1, y - 1));
			result.push_back(Coord(x - 1, y));
			result.push_back(Coord(x + 1, y));
		}
		else
		{
			result.push_back(Coord(x - 1, y - 1));
			result.push_back(Coord(x, y - 1));
			result.push_back(Coord(x + 1, y - 1));
			result.push_back(Coord(x - 1, y));
			result.push_back(Coord(x + 1, y));
			result.push_back(Coord(x - 1, y + 1));
			result.push_back(Coord(x, y + 1));
			result.push_back(Coord(x + 1, y + 1));
		}
	}

	return result;
}

void ms::Field::tag(int x, int y)
{
	// Check if the field has been generated
	if (!generated)
	{
		return;  // You cannot tag a cell until you dig your first cell
	}

	// Check for exceptions
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
	{
		return;
	}

	// If the cell has already been revealed or the game is over
	if (std::find(revealed_cells.begin(), revealed_cells.end(), Coord(x, y)) != revealed_cells.end() ||
		game_over)
	{
		return;
	}

	// Find the cell in the list of tagged cells
	std::vector <Coord>::iterator iter {std::find(tagged_cells.begin(), tagged_cells.end(), Coord(x, y))};
	
	//If the cell has already been tagged
	if (iter != tagged_cells.end())
	{
		// Then untag it (delete specified coordinates from the list)
		tagged_cells.erase(iter);
		return;
	}

	// Add the cell to the list of the tagged cells
	tagged_cells.push_back(Coord(x, y));

	// Check for victory
	check_victory();
}

ms::Field::State ms::Field::get_cell(int x, int y)
{
	// Check for exceptions
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
	{
		throw(std::string("ms::Field::get_cell(): the specified coordinate is out of the border of the field"));
	}

	std::vector <Coord>::iterator cell_iter, tagged_cell_iter;
	
	// Try to find the cell in the list of revealed cells
	cell_iter = std::find(revealed_cells.begin(), revealed_cells.end(), Coord(x, y));
	
	if (cell_iter == revealed_cells.end())  // If the cell has not been revealed yet
	{
		// Try to find the cell in the list of tagged cells
		tagged_cell_iter = std::find(tagged_cells.begin(), tagged_cells.end(), Coord(x, y));

		if (game_over && !victory)  // If the player has lost
		{
			if (tagged_cell_iter == tagged_cells.end())  // If the cell has not been tagged yet
			{
				if (bombs_data[y][x])  // If the cell contains a bomb
				{
					return BOMB;
				}
				else
				{
					return UNREVEALED;
				}
			}
			else if (bombs_data[y][x])  // If the cell actually contains a bomb
			{
				return TAGGED;
			}
			else
			{
				return INCORRECTLY_TAGGED;
			}
		}
		else
		{
			if (tagged_cell_iter == tagged_cells.end())  // If the cell has not been tagged yet
			{
				return UNREVEALED;  // The cell is not revealed
			}
			else
			{
				return TAGGED;  // The cell is tagged
			}
		}
	}
	else
	{
		if (game_over && !victory && fatal_cell.x == x && fatal_cell.y == y)
		{
			return FATAL;  // The player was blown up at this cell
		}
		else
		{
			return State(field_data[y][x]);  // The cell is revealed
		}
	}
}

ms::Coord ms::Field::size() const
{
	return Coord(WIDTH, HEIGHT);
}

void ms::Field::check_victory()
{
	if (tagged_cells.size() == total_bombs && revealed_cells.size() == WIDTH * HEIGHT - total_bombs)
	{
		bool tagged_incorrectly {false};
		std::vector <Coord>::const_iterator iter;

		// Check if all tagged cells actually contain bombs
		for (iter = tagged_cells.begin(); iter != tagged_cells.end(); iter++)
		{
			if (!bombs_data[iter->y][iter->x])
			{
				tagged_incorrectly = true;
				break;
			}
		}
		
		// If all bombs are tagged correctly
		if (!tagged_incorrectly)
		{
			// Declare the victory
			victory = true;
			game_over = true;

			// Debug
#ifdef MINESWEEPER_DEBUG
			std::cout << "Victory!" << std::endl;
#endif
		}
	}
}

bool ms::Field::is_game_over() const
{
	return game_over;
}

bool ms::Field::is_victory() const
{
	return victory;
}

int ms::Field::get_revealed_cells_count() const
{
	return revealed_cells.size();
}

int ms::Field::get_found_bombs_count() const
{
	std::vector <Coord>::const_iterator iter;
	int bombs_count {0};

	for (iter = tagged_cells.begin(); iter != tagged_cells.end(); iter++)
	{
		if (bombs_data[iter->y][iter->x])
		{
			bombs_count++;
		}
	}

	return bombs_count;
}

ms::Coord ms::Field::get_size() const
{
	return Coord(bombs_data[0].size(), bombs_data.size());
}

int ms::Field::get_bombs_count() const
{
	return total_bombs;
}

ms::Coord ms::Field::get_first_cell() const
{
	if (!generated)
	{
		throw std::string("No cells has been dug yet");
	}

	return first_cell;
}

int ms::Field::get_cells_per_bomb() const
{
	return CPB;
}

void ms::Field::save() const
{
	// Do not save the game if the field has not been generated
	if (!generated)
	{
		return;
	}

	std::ofstream stream(SAVE_FILENAME, std::ios::binary);  // Open an output stream

	// Get some data
	std::time_t raw_time = std::time(NULL);
	int game_time = get_time();
	int click_buffer_size = click_buffer.size();
	int flags_vector_size = tagged_cells.size();

	stream.write((char *) &raw_time, sizeof(std::time_t));  // Write current time
	stream.write((char *) &game_time, sizeof(int));  // Write current game time
	stream.write((char *) &WIDTH, sizeof(int));  // Field width
	stream.write((char *) &HEIGHT, sizeof(int));  // Field height
	stream.write((char *) &CPB, sizeof(int));  // Cells per bomb
	stream.write((char *) &SEED, sizeof(int));  // Seed

	// Write the clickbuffer
	stream.write((char *) &click_buffer_size, sizeof(int));  // Write its size

	std::vector <Coord>::const_iterator iter;  // And then its content
	for (iter = click_buffer.begin(); iter != click_buffer.end(); iter++)
	{
		stream.write((char *) &iter->x, sizeof(int));
		stream.write((char *) &iter->y, sizeof(int));
	}

	// Write the vector of tagged cells
	stream.write((char *) &flags_vector_size, sizeof(int));  // Write its size
	for (iter = tagged_cells.begin(); iter != tagged_cells.end(); iter++)  // And its content
	{
		stream.write((char *) &iter->x, sizeof(int));
		stream.write((char *) &iter->y, sizeof(int));
	}

	stream.close();
}

int ms::Field::get_time() const
{
	if (generated)
	{
		return clock.getElapsedTime().asSeconds() + start_time;
	}
	else
	{
		return 0;
	}
}

bool ms::Field::load()
{
	// Clear all containers
	bombs_data.clear();
	field_data.clear();
	revealed_cells.clear();
	tagged_cells.clear();
	click_buffer.clear();

	// Open the file
	std::ifstream stream(SAVE_FILENAME, std::ios::binary);

	if (!stream)  // If unable to open the file
	{
		return false;
	}
	
	std::time_t raw_time;

	stream.read((char *) &raw_time, sizeof(std::time_t));  // Read the time of the last change
	stream.read((char *) &start_time, sizeof(int));  // Game time
	stream.read((char *) &WIDTH, sizeof(int));  // Width of the field
	stream.read((char *) &HEIGHT, sizeof(int));  // Height of the field
	stream.read((char *) &CPB, sizeof(int));  // Cells per bomb
	stream.read((char *) &SEED, sizeof(int));  // Seed

	// Read the clickbuffer
	int click_buffer_size;
	stream.read((char *) &click_buffer_size, sizeof(int));  // Read its size
	for (int i = 0; i < click_buffer_size; i++)  // And then its content
	{
		Coord crd(0, 0);  // Read the cell coordinates
		stream.read((char *) &crd.x, sizeof(int));
		stream.read((char *) &crd.y, sizeof(int));
		click_buffer.push_back(crd);  // Push the coordinate into the buffer
	}

	// Read the vector of tagged cells
	int flags_vector_size;
	stream.read((char *) &flags_vector_size, sizeof(int));  // Read its size
	for (int i = 0; i < flags_vector_size; i++)  // And then its content
	{
		Coord crd(0, 0);  // Read the cell coordinates
		stream.read((char *) &crd.x, sizeof(int));
		stream.read((char *) &crd.y, sizeof(int));
		tagged_cells.push_back(crd);  // Push the coordinate into the buffer
	}

	stream.close();

	// Restore the state of the field
	std::vector <Coord>::iterator iter;
	for (iter = click_buffer.begin(); iter != click_buffer.end(); iter++)
	{
		dig(iter->x, iter->y, false);
	}

	return true;
}

int ms::Field::get_seed() const
{
	return SEED;
}

void ms::Field::delete_save() const
{
	std::remove(SAVE_FILENAME.c_str());
}