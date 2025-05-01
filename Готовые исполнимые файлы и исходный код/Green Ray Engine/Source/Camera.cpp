#include "Camera.h"

gr::Camera gr::cam(0, 0);

const float gr::Camera::MAX_FOLLOW_DISTANCE = 1;
const float gr::Camera::MAX_SPEED = 5;

gr::Camera::Camera(float ax, float ay):
	x(ax), y(ay), target_x(ax), target_y(ay), vx(0), vy(0)
{}

bool gr::Camera::crdToPix(float ax, float ay, int &rx, int &ry, float w, float h)
{
	// Calculate coordinates of the left-bottom corner of the object relative to the camera
	ax -= x;
	ay -= y;
	// Now ax and ay store coordinates relative to the camera (to the center of view)

	// Calculate the position on screen
	rx = ax * TILE_SIZE + WND_WIDTH / 2;
	ry = WND_HEIGHT / 2 - (ay + h) * TILE_SIZE;

	// Check if the object is captured by the camera
	return !( (rx + w * TILE_SIZE) <= 0 || rx >= WND_WIDTH ||
			  (ry + h * TILE_SIZE) <= 0 || ry >= WND_HEIGHT );
}

void gr::Camera::moveSmoothlyTo(float ax, float ay)
{
	target_x = ax;
	target_y = ay;
}

void gr::Camera::update()
{
	// Calculate distance for both coordinates
	float dist_x = target_x - x;
	float dist_y = target_y - y;

	// Update the camera coordinates and velocity
	if (abs(dist_x) > MAX_FOLLOW_DISTANCE)  // If the camera too far
	{
		if (dist_x > 0)
		{
			x = target_x - MAX_FOLLOW_DISTANCE;
			vx = MAX_SPEED;
		}
		else
		{
			x = target_x + MAX_FOLLOW_DISTANCE;
			vx = -MAX_SPEED;
		}
	}
	else
	{
		vx = MAX_SPEED * (dist_x / MAX_FOLLOW_DISTANCE);
	}

	if (abs(dist_y) > MAX_FOLLOW_DISTANCE)  // If the camera is too far
	{
		if (dist_y > 0)
		{
			y = target_y - MAX_FOLLOW_DISTANCE;
			vy = MAX_SPEED;
		}
		else
		{
			y = target_y + MAX_FOLLOW_DISTANCE;
			vy = -MAX_SPEED;
		}
	}
	else
	{
		vy = MAX_SPEED * (dist_y / MAX_FOLLOW_DISTANCE);
	}

	// Move the camera according to its velocity
	x += vx * game_tick.get_period();
	y += vy * game_tick.get_period();
}

void gr::Camera::moveTo(float ax, float ay)
{
	x = ax;
	y = ay;
}