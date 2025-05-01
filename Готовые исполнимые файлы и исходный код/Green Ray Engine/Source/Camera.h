#pragma once
#include "GlobalData.h"
#include "GameTick.h"

namespace gr
{

class Camera
{
private:
	float x, y;  // View center coordinates
	float vx, vy;  // Current camera velocity
	float target_x, target_y;  // Target coordinates
	/* The time that is taken to lessen the distance between the camera and the target by 1/2 */
	static const float MAX_FOLLOW_DISTANCE;  // In tiles
	static const float MAX_SPEED;  // In tiles per second
public:
	Camera(float x=0, float y=0);  // Constructor

	/* Converts coordinates of an object to coordinates on screen in pixels
	x: x-coordinate of left bottom corner of the object
	y: y-coordinate of left bottom corner of the object
	rx: x-coordinate of left top corner of the object on screen in pixels (result)
	ry: y-coordinate of left top corner of the object on screen in pixels (result)
	w: width of the object in tiles
	h: height of the object in tiles
	returns true if the object is captured by the camera, returns false otherwise */
	bool crdToPix(float x, float y, int &rx, int &ry, float w=1, float h=1);

	/* Starts an animation of smooth moving to the specified location */
	void moveSmoothlyTo(float x, float y);

	void update();  // Updates camera movement

	void moveTo(float x, float y);  // Immediatly teleports the camera to the specified coordinates
};

extern Camera cam;  // Predefined Camera object

}