#pragma once
#include "Entity.h"
#include "Switch.h"
#include "Control.h"

namespace gr
{

class Player: public Entity
{
protected:
	static const float WALK_VEL;  // Walking velocity in tiles per second
	static const float WALK_ACC;  // Walking acceleration
	static const float FREE_FALL_ACC;  // Free fall acceleration
	static const float JUMP_VEL;  // Vertical jumping velocity
	static const float PENETRATE_THRESHOLD;
	static const float AIR_ACC;  // Horizontal acceleration while airborne
	static const float LADDER_ACC;  // Vertical acceleration while using ladder
	static const float LADDER_VEL;  // Climbing on ladder velocity
	bool hit_right, hit_left, hit_top, hit_bottom;  // Tile interaction flags
	bool can_use_ladder, is_on_ladder;  // Ladder interaction flags
	float pre_vx, pre_vy;  // Speed at a previous frame

	// Controls
	Control up_control, left_control, down_control, right_control, hang_control, jump_control;

	// Switches
	Switch hang_switch;
public:
	Player(float x=0, float y=0, float w=0.5, float h=1.75);  // Constructor

	void hor_update();  // Updates an entity's horizontal movement

	void ver_update();  // Updates an entity's vertical movement

	void hor_collision();  // Detects collision with tiles horizontally

	void ver_collision();  // Detects collision with tiles vertically

	void update();  // Updates an entity's movement in all directions

	void debug();  // Updates debug info

	void check_interact();  // Checks all blocks the player touches and updates interaction flags

	void update_switches();  // Updates all key switches

	const float &pick_hor_acc() const;  // Returns current normal horizontal acceleration depending on flags

	const float &pick_hor_vel() const;  // Returns current normal horizontal speed depending on flags

	void update_controls();  // Updates all controls

	/*
	* returns:
	* true: hor_update must be called before ver_update
	* false: ver_update must be called before hor_update
	*/
	bool move_check_order() const;  // Defines in which order move check should be performed

	void save_velocity();  // Backups velocity values
};

}