#pragma once
#include <SFML/Graphics.hpp>

namespace ms
{

class GradientRect
{
private:
	sf::Vertex vertexes[4];
	int origin_x, origin_y;
	int width, height;
public:
	/* Constructor
	   Creates a gradient object
	   width, height: size of the rectangle
	   color1..color4: colors of the vertexes beginning from the left top vertex clockwise
	*/
	GradientRect(int width, int height, sf::Color color1, sf::Color color2, sf::Color color3, sf::Color color4);

	/* Sets the origin of the object */
	void setOrigin(int x, int y);

	/* Sets the position of the rectangle */
	void setPosition(int x, int y);

	/* Draws the gradient at the window */
	void draw(sf::RenderWindow &window) const;
};

}