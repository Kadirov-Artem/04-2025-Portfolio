#include "Player.h"

const float gr::Player::WALK_VEL = 5;
const float gr::Player::WALK_ACC = 50;
const float gr::Player::FREE_FALL_ACC = 19.62;
const float gr::Player::JUMP_VEL = 7.5;
const float gr::Player::PENETRATE_THRESHOLD = 0.001;
const float gr::Player::AIR_ACC = 10;
const float gr::Player::LADDER_ACC = 50;
const float gr::Player::LADDER_VEL = 3;

gr::Player::Player(float ax, float ay, float aw, float ah):
	Entity(ax, ay, aw, ah), hit_left(true), hit_right(true), hit_top(true),
	hit_bottom(true), pre_vx(0), is_on_ladder(false), can_use_ladder(false),
	up_control(sf::Keyboard::Key::W), left_control(sf::Keyboard::Key::A),
	down_control(sf::Keyboard::Key::S),	right_control(sf::Keyboard::Key::D),
	hang_control(sf::Keyboard::Key::LShift), jump_control(sf::Keyboard::Key::Space),
	hang_switch(&hang_control), pre_vy(0)
{}

void gr::Player::hor_update()
{
	// Update horizontal velocity
	if ( left_control.state() && !right_control.state() )
	{
		if (!hit_left)  // If there is no obstacle on the left
		{
			//if (!is_moving_hor)
			//{
				is_moving_hor = true;
			//}			

			if(vx > -pick_hor_vel())
			{
				vx += -(pick_hor_acc()) * game_tick.get_period();
			}
			else
			{
				vx -= -(pick_hor_acc()) * game_tick.get_period();
			}
		}
	}
	else if ( !left_control.state() && right_control.state() )
	{
		if (!hit_right)
		{
			//if (!is_moving_hor)
			//{
				is_moving_hor = true;
			//}

			if (vx < pick_hor_vel())
			{
				vx += (pick_hor_acc()) * game_tick.get_period();
			}
			else
			{
				vx -= (pick_hor_acc()) * game_tick.get_period();
			}
		}
	}
	else
	{
		if (is_moving_hor)
		{
			if (vx > 0)
			{
				vx -= (pick_hor_acc()) * game_tick.get_period();
			}
			else
			{
				vx += (pick_hor_acc()) * game_tick.get_period();
			}
			
			if (vx < 0 && pre_vx > 0 || vx > 0 && pre_vx < 0)
			{
				vx = 0;
				is_moving_hor = false;
			}
		}
	}

	// Update horizontal coordinate
	if (vx < 0 && !hit_left || vx > 0 && !hit_right)
	{
		x += vx * game_tick.get_period();
	}

	// Reset hit flags
	//hit_left = hit_right = false;

	// Detect horizontal collision with tiles
	hor_collision();
}

void gr::Player::ver_update()
{
	// Update vertical velocity
	if ( hit_bottom && jump_control.state() )
	{
		// Jumping (can only be performed when is not airborne)
		//if (!is_moving_ver)
		//{
			is_moving_ver = true;
		//}

		vy = JUMP_VEL;
	}
	else if (!hit_bottom)
	{
		if (!is_moving_ver && !is_on_ladder)
		{
			is_moving_ver = true;
		}

		if (!is_on_ladder)
		{
			vy -= FREE_FALL_ACC * game_tick.get_period();
		}
	}

	// Update ladder climbing
	if (is_on_ladder)
	{
		if ( down_control.state() && !( up_control.state() || jump_control.state() ) )
		{
			if (!hit_bottom)
			{
				//if (!is_moving_ver)
				//{
					is_moving_ver = true;
				//}

				// Change speed
				if (vy > -LADDER_VEL)
				{
					vy += -LADDER_ACC * game_tick.get_period();
				}
				else
				{
					vy -= -LADDER_ACC * game_tick.get_period();
				}
			}
		}
		else if ( !down_control.state() && ( up_control.state() || jump_control.state() ) )
		{
			if (!hit_top)
			{
				//if (!is_moving_ver)
				//{
					is_moving_ver = true;
				//}

				// Change speed
				if (vy < LADDER_VEL)
				{
					vy += LADDER_ACC * game_tick.get_period();
				}
				else
				{
					vy -= LADDER_ACC * game_tick.get_period();
				}
			}
		}
		else
		{
			if (is_moving_ver)
			{
				if (vy > 0)
				{
					vy -= LADDER_ACC * game_tick.get_period();
				}
				else
				{
					vy += LADDER_ACC * game_tick.get_period();
				}

				if (vy < 0 && pre_vy > 0 || vy > 0 && pre_vy < 0)
				{
					vy = 0;
					is_moving_ver = false;
				}
			}
		}
	}

	// Update vertical coordinate
	if (vy < 0 && !hit_bottom || vy > 0 && !hit_top)
	{
		y += vy * game_tick.get_period();
	}

	// Reset hit flags
	//hit_bottom = hit_top = false;

	// Detect vertical collision with tiles
	ver_collision();
}

void gr::Player::hor_collision()
{
	// Get vertical tile range
	const int bottom_tile = floor(y);
	const int top_tile = floor(y + h - PENETRATE_THRESHOLD);

	// Collision with left obstacles
	if (is_moving_hor)
	{
		if (vx < 0)
		{
			int tile_x = floor(x);  // Get the tiles' x-coordinate

			// Now we have a column of tiles that are to be checked
			bool collision_detected = false;
			for (int tile_y = bottom_tile; tile_y <= top_tile; tile_y++)
			{
				switch (getTile(tile_x, tile_y)->group())
				{
				case gr::TileGroup::DEFAULT:	
					collision_detected = true;
					break;
				default:
					break;
				}
			}

			// In case of collision teleport the entity out of tiles and set horizontal speed to zero
			if (collision_detected)
			{
				hit_left = true;  // Raise the left collision flag
				is_moving_hor = false;  // Horizontal moving flag down
				x = tile_x + 1;
				vx = 0;
			}
			else  // If there was not a collision
			{
				hit_left = hit_right = false;  // Reset all collision flags
			}
		}
		// Collision with right obstacles
		else if (vx > 0)
		{
			int tile_x = floor(x + w);  // Get the tiles' x-coordinate

			// Now we have a column of tiles that are to be checked
			bool collision_detected = false;
			for (int tile_y = bottom_tile; tile_y <= top_tile; tile_y++)
			{
				switch (getTile(tile_x, tile_y)->group())
				{
				case gr::TileGroup::DEFAULT:	
					collision_detected = true;
					break;
				default:
					break;
				}
			}

			// In case of collision teleport the entity out of tiles and set horizontal speed to zero
			if (collision_detected)
			{
				hit_right = true;  // Raise the right collision flag
				is_moving_hor = false;  // Horizontal moving flag down
				x = tile_x - w;
				vx = 0;
			}
			else  // If there was not a collision
			{
				hit_left = hit_right = false;  // Reset all collision flags
			}
		}
	}
	else  // If the entity is not moving horizontally
	{
		// Check tiles on the left from the entity
		int tile_x = floor(x);  // Get the tiles' x-coordinate
		bool tile_detected = false;
		for (int tile_y = bottom_tile; tile_y <= top_tile; tile_y++)
		{
			switch (getTile(tile_x - 1, tile_y)->group())
			{
			case gr::TileGroup::DEFAULT:	
				tile_detected = true;
				break;
			default:
				break;
			}
		}

		/* If there aren't any tiles on the left from the entity, unset the "hit_left" flag
		so as to let the object go left */
		if (!tile_detected)
		{
			hit_left = false;
		}

		// Check tiles on the right from the entity
		tile_x = floor(x + w - PENETRATE_THRESHOLD);  // Get the tiles' x-coordinate
		tile_detected = false;
		for (int tile_y = bottom_tile; tile_y <= top_tile; tile_y++)
		{
			switch (getTile(tile_x + 1, tile_y)->group())
			{
			case gr::TileGroup::DEFAULT:	
				tile_detected = true;
				break;
			default:
				break;
			}
		}

		/* If there aren't any tiles on the right from the entity, unset the "hit_right" flag
		so as to let the object go right */
		if (!tile_detected)
		{
			hit_right = false;
		}
	}
}

void gr::Player::ver_collision()
{
	// Get horizontal tile range
	const int left_tile = floor(x);
	const int right_tile = floor(x + w - PENETRATE_THRESHOLD);

	// Collision with bottom obstacles
	if (is_moving_ver)
	{
		if (vy < 0)
		{
			int tile_y = floor(y);  // Get the tiles' y-coordinate

			// Now we have a row of tiles that are to be checked
			bool collision_detected = false;
			for (int tile_x = left_tile; tile_x <= right_tile; tile_x++)
			{
				switch (getTile(tile_x, tile_y)->group())
				{
				case gr::TileGroup::DEFAULT:	
					collision_detected = true;
					break; 
				default:
					break;
				}
			}

			// In case of collision teleport the entity out of tiles and set vertical speed to zero
			if (collision_detected)
			{
				hit_bottom = true;  // Raise the bottom collision flag
				is_moving_ver = false;  // Vertical moving flag down
				y = tile_y + 1;
				vy = 0;
			} 
			else  // If there was not a collision
			{
				hit_bottom = hit_top = false;  // Reset all collision flags
			}
		}
		// Collision with top obstacles
		if (vy > 0)
		{
			int tile_y = floor(y + h);  // Get the tiles' y-coordinate

			// Now we have a row of tiles that are to be checked
			bool collision_detected = false;
			for (int tile_x = left_tile; tile_x <= right_tile; tile_x++)
			{
				switch (getTile(tile_x, tile_y)->group())
				{
				case gr::TileGroup::DEFAULT:	
					collision_detected = true;
					break;
				default:
					break;
				}
			}

			// In case of collision teleport the entity out of tiles and set vertical speed to zero
			if (collision_detected)
			{
				hit_top = true;  // Raise the bottom collision flag
				is_moving_ver = false;  // Vertical moving flag down
				y = tile_y - h;
				vy = 0;
			}
			else
			{
				hit_bottom = hit_top = false;
			}
		}
	}
	else  // If the entity is not moving vertically
	{
		// Check tiles beneath the entity
		int tile_y = floor(y);  // Get the tiles' y-coordinate		
		bool tile_detected = false;
		for (int tile_x = left_tile; tile_x <= right_tile; tile_x++)
		{
			switch (getTile(tile_x, tile_y - 1)->group())
			{
			case gr::TileGroup::DEFAULT:	
				tile_detected = true;
				break;
			default:
				break;
			}
		}

		/* If there aren't any tiles beneath the entity, unset the "hit_bottom" flag
		so as to let the object fall */
		if (!tile_detected)
		{
			hit_bottom = false;
		}

		// Check tiles above the entity
		tile_y = floor(y + h - PENETRATE_THRESHOLD);  // Get the tiles' y-coordinate		
		tile_detected = false;
		for (int tile_x = left_tile; tile_x <= right_tile; tile_x++)
		{
			switch (getTile(tile_x, tile_y + 1)->group())
			{
			case gr::TileGroup::DEFAULT:	
				tile_detected = true;
				break;
			default:
				break;
			}
		}

		/* If there aren't any tiles above the entity, unset the "hit_top" flag
		so as to let the object go up */
		if (!tile_detected)
		{
			hit_top = false;
		}
	}
}

void gr::Player::update()
{
	update_controls();
	update_switches();

	save_velocity();

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

	// Update tile interaction
	check_interact();

	// Update camera movement
	cam.moveSmoothlyTo(x + w / 2, y + w / 2);
}

void gr::Player::debug()
{
	gr::Entity::debug();  // Call ancestor's method

	// Append debug information
	debug_stream << std::boolalpha
		<< "hit_left = " << hit_left << std::endl
		<< "hit_right = " << hit_right << std::endl
		<< "hit_bottom = " << hit_bottom << std::endl
		<< "hit_top = " << hit_top << std::endl
		<< "can_use_ladder = " << can_use_ladder << std::endl
		<< "is_on_ladder = " << is_on_ladder << std::endl;
}

void gr::Player::check_interact()
{
	// Get vertical tile range
	const int bottom_tile = floor(y);
	const int top_tile = floor(y + h - PENETRATE_THRESHOLD);

	// Get horizontal tile range
	const int left_tile = floor(x);
	const int right_tile = floor(x + w - PENETRATE_THRESHOLD);

	// Search flags
	bool ladders_found {false};

	// Check for interaction with tiles
	for (int tile_y = bottom_tile; tile_y <= top_tile; tile_y++)
	{
		for (int tile_x = left_tile; tile_x <= right_tile; tile_x++)
		{
			if (!ladders_found && getTile(tile_x, tile_y)->group() == gr::TileGroup::CLIMBABLE)
			{
				ladders_found = true;
				can_use_ladder = true;
			}
		}
	}

	// Unset flags
	if (!ladders_found)
	{
		can_use_ladder = false;
		is_on_ladder = false;
		hang_switch.set_state(false);
	}
	
	// Update switch flags
	if (can_use_ladder)
	{
		is_on_ladder = hang_switch.state();
	}
}

void gr::Player::update_switches()
{
	hang_switch.update();
}

const float &gr::Player::pick_hor_acc() const
{
	if (is_on_ladder || hit_bottom)
	{
		return WALK_ACC;
	}
	else
	{
		return AIR_ACC;
	}
}

void gr::Player::update_controls()
{
	up_control.update();
	left_control.update();
	down_control.update();
	right_control.update();
	hang_control.update();
	jump_control.update();
}

const float &gr::Player::pick_hor_vel() const
{
	return WALK_VEL;
}

bool gr::Player::move_check_order() const
{
	// Find out in which direction movement should be checked first
	float px, py;  // Coordinates of the corner of the hitbox that matches
	// the entity's movement direction

	float dx, dy;  // Distance from the point to respective sides of a tile

	if (vx < 0)
	{
		if (vy < 0)
		{
			// If the entity moves towards left-down
			px = x;
			py = y;

			dx = ceil(px) - px;
			dy = ceil(py) - py;
		}
		else
		{
			// If the entity moves towards left-up
			px = x;
			py = y + h;

			dx = ceil(px) - px;
			dy = py - floor(py);
		}
	}
	else
	{
		if (vy < 0)
		{
			// If the entity moves towards right-down
			px = x + w;
			py = y;

			dx = px - floor(px);
			dy = ceil(py) - py;
		}
		else
		{
			// If the entity moves towards right-up
			px = x + w;
			py = y + h;

			dx = px - floor(px);
			dy = py - floor(py);
		}
	}

	return dx < dy;
}

void gr::Player::save_velocity()
{
	// Save speed value
	pre_vx = vx;
	pre_vy = vy;
}