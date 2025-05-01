#pragma once
#include <SFML/Graphics.hpp>
#include "Control.h"

namespace gr
{

/* Class for using keys as switches
A switch turns on and turns off only when
a binded key is released and then pressed */
class Switch
{
private:
	Control *key;
	bool key_state, previous_key_state, switch_state;
public:
	Switch(Control *key, bool first_state=false);  // Constructor

	void update();  // Updates the switch state

	bool state() const;  // Returns the state of the switch

	inline void set_state(bool new_state) {switch_state = new_state;} // Sets the switch's state
};

}