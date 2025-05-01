#pragma once
#include <SFML/Graphics.hpp>

namespace gr
{

/* All game mechanics and animations, except for physics and the player, are bound to ticks */
class GameTick
{
private:
	static const int TICK_DURATION;  // Tick duration in milliseconds
	sf::Clock global_clock, period_clock;
	int global_tick;  // 1 tick = 50 ms by default
	float elapsed_time;  // Time between calls of "update" function in seconds
	bool is_paused;  // Defines whether the game is paused
public:
	GameTick();  // Constructor

	void update();  // Updates the current tick number

	int get_tick();  // Returns the current tick number

	float get_period();  // Returns "elapsed_time"

	void pause();  // Pauses the game

	void resume();  // Resumes the game
};

extern GameTick game_tick;  // Predefined object

}