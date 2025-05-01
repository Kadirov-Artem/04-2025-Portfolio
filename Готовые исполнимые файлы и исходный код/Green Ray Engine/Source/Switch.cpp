#include "Switch.h"

gr::Switch::Switch(Control *akey, bool afirst_state):
	key(akey), key_state(false), previous_key_state(false), switch_state(afirst_state)
{}

void gr::Switch::update()
{
	previous_key_state = key_state;
	key_state = key->state();
	if (!previous_key_state && key_state)
	{
		switch_state = !switch_state;
	}
}

bool gr::Switch::state() const
{
	return switch_state;
}