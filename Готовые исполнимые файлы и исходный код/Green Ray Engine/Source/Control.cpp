#include "Control.h"

gr::Control::Control(sf::Keyboard::Key akey, bool akey_state):
	key(akey), key_state(akey_state)
{}

void gr::Control::update()
{
	key_state = sf::Keyboard::isKeyPressed(key);
}

bool gr::Control::state() const
{
	return key_state;
}

void gr::Control::set_state(bool anew_state)
{
	key_state = anew_state;
}