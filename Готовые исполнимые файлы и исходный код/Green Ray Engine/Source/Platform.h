#pragma once
#include <vector>
#include "Player.h"
#include "FltCrd.h"

namespace gr
{

class Platform: public Player
{
protected:
	static const float MOVE_VEL;  // Speed of a platform in tiles per second
	std::vector <FltCrd> route;  // Cycle of points that are reached by the platform one by one
public:
	Platform(float x, float y, std::vector <FltCrd> route);

	void hor_update();  // Updates an entity's horizontal movement

	void ver_update();  // Updates an entity's vertical movement
	
	void debug();  // Does nothing for this class of objects

	const float &pick_hor_vel() const;  // Returns current acceleration depending on flags

	void update_controls();  // Updates all controls

	void update_switches();  // Does nothing for this type of objects

	void check_interact();  // Doew nothing for this type of objects

	void update();  // Updates an entity's movement in all directions
};

}