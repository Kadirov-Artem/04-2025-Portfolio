#include "GameTick.h"

const int gr::GameTick::TICK_DURATION = 50;
gr::GameTick gr::game_tick;

gr::GameTick::GameTick():
	global_tick(0), elapsed_time(0), is_paused(false)
{
	global_clock.restart();
	period_clock.restart();
}

void gr::GameTick::update()
{
	global_tick = global_clock.getElapsedTime().asMilliseconds() / TICK_DURATION;
	elapsed_time = is_paused ? 0 : period_clock.getElapsedTime().asSeconds();
	period_clock.restart();
}

int gr::GameTick::get_tick()
{
	return global_tick;
}

float gr::GameTick::get_period()
{
	return elapsed_time;
}

void gr::GameTick::pause()
{
	is_paused = true;
}

void gr::GameTick::resume()
{
	is_paused = false;
}