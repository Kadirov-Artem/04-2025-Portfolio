#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <SFML/System.hpp>
//#define MINESWEEPER_DEBUG

namespace ms
{

struct Coord
{
	int x;
	int y;

	Coord(int ax, int ay): x(ax), y(ay) {}
	bool operator ==(const Coord &right) const;
};

class Field
{
public:
	enum State {FATAL = -5, INCORRECTLY_TAGGED = -4, UNREVEALED = -3, TAGGED = -2, BOMB = -1,
				ZERO = 0, ONE = 1, TWO = 2, THREE = 3, FOUR = 4,
				FIVE = 5, SIX = 6, SEVEN = 7, EIGHT = 8};
private:
	static const std::string SAVE_FILENAME;

	int WIDTH, HEIGHT, CPB, SEED;
	std::vector <std::vector <bool> > bombs_data;  // 2d-array: 1 - bomb, 0 - free
	std::vector <std::vector <char> > field_data;  // 2d-array: -1 - bomb, numbers 0-8 - free
	std::vector <Coord> revealed_cells;  // List of revealed cells
	std::vector <Coord> tagged_cells;  // List of tagged cells
	std::vector <Coord> click_buffer;  // List of left-clicked cells
	int total_bombs;
	bool game_over;
	bool victory;
	bool generated;
	Coord first_cell;  // First dug cell
	Coord fatal_cell;  // Cell at which the player was blown up
	sf::Clock clock;  // Game time
	int start_time;  // Game start time
public:
	/* Constructor
	   width: width of the field
	   height: height of the field
	   seed: seed for pseudorandom generator
	   cells_per_bomb: average number of cells one of them contains a bomb
	*/
	Field(int width, int height, int seed, int cells_per_bomb=6);

	/* Constructor
	   Creates an empty game object
	   The "load" function is supposed to be called right after creating the object
	   Calling any other functions before the "load" function will lead to undefined behaviour
	*/
	Field();

	/* Digs the cell
	   If the cell contains a bomb, the game is over
	   If not, the number of neighboring bombs is revealed
	   x, y: the cell coordinates
	   save_to_buffer: if false, the click will not be saved to the click buffer
	*/
	void dig(int x, int y, bool save_to_buffer=true);

	/* Finds all neighboring (also corner-neighboring) cells for the specified cell
	   x, y: the coordinates of the specified cell
	   returns: an std::vector <Coord> containing the coordinates of the needed cells
	*/
	std::vector <Coord> find_neighboring(int x, int y) const;

	/* Tags the specified cell as a bomb
	   The player wins if all bombs are found
	   and no extra flags are set
	   x, y: the coordinates of the specified cell
	*/
	void tag(int x, int y);

	/* Returns a state of the specified cell:
	   -3: not revealed
	   -2: tagged
	   -1: bomb
	   0-8: revealed
	*/
	State get_cell(int x, int y);

	/* Returns the size of the field */
	Coord size() const;

	/* Returns true if the game is over,
	   returns false otherwise
	*/
	bool is_game_over() const;

	/* Returns true if player has won,
	   returns false otherwise
	*/
	bool is_victory() const;

	/* Returns the number of revealed cells */
	int get_revealed_cells_count() const;

	/* Returns the number of found bombs */
	int get_found_bombs_count() const;

	/* Returns the size of the field */
	Coord get_size() const;

	/* Returns the total number of bombs  */
	int get_bombs_count() const;

	/* Generates the field
	   Does not place a bomb at the specified cell
	*/
	void generate(int x, int y);

	/* Returns the first dug cell coordinates */
	Coord get_first_cell() const;

	/* Returns the number of cells per bomb */
	int get_cells_per_bomb() const;

	/* Saves the game */
	void save() const;

	/* If possible, loads the game from file */
	bool load();

	/* Returns the time since the first click */
	int get_time() const;

	/* Returns the generator seed */
	int get_seed() const;

	/* Deletes the game save file */
	void delete_save() const;
private:
	/* Checks if player has won */
	void check_victory();
};

}