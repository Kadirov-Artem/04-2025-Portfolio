#pragma once
#include <SFML/Graphics.hpp>

namespace gr
{

class Control
{
private:
	const sf::Keyboard::Key key;
	bool key_state;
public:
	Control(sf::Keyboard::Key key, bool key_state=false);  // Constructor

	void update();

	bool state() const;

	void set_state(bool new_state);
};

}