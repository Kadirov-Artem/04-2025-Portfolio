#include "Platform.h"

const float gr::Platform::MOVE_VEL= 3;

gr::Platform::Platform(float ax, float ay, std::vector <FltCrd> aroute):
	Player(ax, ay, 1, 0.125)
{}

void gr::Platform::hor_update()
{
	// Update horizontal velocity
	if (left_control.state())
	{
		if (!hit_left)  // If there is no obstacle on the left
		{
			is_moving_hor = true;
			vx = -pick_hor_vel();
		}
	}
	else if (right_control.state())
	{
		if (!hit_right)  // If there is no obstacle on the right
		{
			is_moving_hor = true;
			vx = pick_hor_vel();
		}
	}
	else
	{
		is_moving_hor = false;
		vx = 0;
	}

	// Update horizontal coordinate
	if (vx < 0 && !hit_left || vx > 0 && !hit_right)
	{
		x += vx * game_tick.get_period();
	}

	// Detect horizontal collision with tiles
	hor_collision();
}

void gr::Platform::ver_update()
{
	// Update vertical velocity
	if (down_control.state())
	{
		if (!hit_bottom)  // If there is no obstacle beneath
		{
			is_moving_ver = true;
			vy = -pick_hor_vel();
		}
	}
	else if (up_control.state())
	{
		if (!hit_top)  // If there is no obstacle above
		{
			is_moving_ver = true;
			vy = pick_hor_vel();
		}
	}
	else
	{
		is_moving_ver = false;
		vy = 0;
	}

	// Update vertical coordinate
	if (vy < 0 && !hit_bottom || vy > 0 && !hit_top)
	{
		y += vy * game_tick.get_period();
	}

	// Detect horizontal collision with tiles
	ver_collision();
}

void gr::Platform::debug()
{
	//gr::Entity::debug();  // Call ancestor's method

	//// Append debug information
	//debug_stream << std::boolalpha
	//	<< "hit_left = " << hit_left << std::endl
	//	<< "hit_right = " << hit_right << std::endl
	//	<< "hit_bottom = " << hit_bottom << std::endl
	//	<< "hit_top = " << hit_top << std::endl
	//	<< "can_use_ladder = " << can_use_ladder << std::endl
	//	<< "is_on_ladder = " << is_on_ladder << std::endl;
}

const float &gr::Platform::pick_hor_vel() const
{
	return MOVE_VEL;
}

void gr::Platform::update_controls()
{
	switch ( (game_tick.get_tick() / 16) % 4 )
	{		
		case 0:
			up_control.set_state(true);
			down_control.set_state(false);
			left_control.set_state(false);
			right_control.set_state(false);
			break;
		case 1:
			up_control.set_state(false);
			down_control.set_state(false);
			left_control.set_state(false);
			right_control.set_state(true);
			break;
		case 2:
			up_control.set_state(false);
			down_control.set_state(true);
			left_control.set_state(false);
			right_control.set_state(false);
			break;
		case 3:
			up_control.set_state(false);
			down_control.set_state(false);
			left_control.set_state(true);
			right_control.set_state(false);
			break;
	}
}

void gr::Platform::update_switches()
{}

void gr::Platform::check_interact()
{}

void gr::Platform::update()
{
	update_controls();
	
	// Save speed value
	pre_vx = vx;
	pre_vy = vy;

	/* That direction should be checked first, in which the entity penetrates a tile less */
	if (move_check_order())
	{
		hor_update();
		ver_update();
	}
	else
	{
		ver_update();
		hor_update();
	}
}