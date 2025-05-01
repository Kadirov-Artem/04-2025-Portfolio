#pragma once
#include <sstream>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include "GlobalData.h"
#include "GameTick.h"
#include "Entity.h"

namespace gr
{

class Debug
{
private:
	sf::Text text;
	std::ostringstream stream;
	int fps;
	bool fps_measured;
	Entity *tracked_entity;
public:
	Debug();  // Constructor

	void update();

	void print(sf::RenderWindow &window);

	void bind_tracked_entity(Entity *entity);
};

extern Debug debug;  // Predefined object

}