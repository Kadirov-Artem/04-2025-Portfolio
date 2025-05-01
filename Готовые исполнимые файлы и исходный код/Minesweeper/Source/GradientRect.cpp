#include "GradientRect.h"

ms::GradientRect::GradientRect(int w, int h, sf::Color col1, sf::Color col2, sf::Color col3, sf::Color col4):
	origin_x(0), origin_y(0),
	width(w), height(h)
{ 
	vertexes[0] = sf::Vertex(sf::Vector2f(0, 0), col1);
	vertexes[1] = sf::Vertex(sf::Vector2f(0, 0), col2);
	vertexes[2] = sf::Vertex(sf::Vector2f(0, 0), col3);
	vertexes[3] = sf::Vertex(sf::Vector2f(0, 0), col4);
}

void ms::GradientRect::setOrigin(int x, int y)
{
	origin_x = x;
	origin_y = y;
}

void ms::GradientRect::setPosition(int x, int y)
{
	vertexes[0].position = sf::Vector2f(x - origin_x, y - origin_y);
	vertexes[1].position = sf::Vector2f(x - origin_x + width - 1, y - origin_y);
	vertexes[2].position = sf::Vector2f(x - origin_x + width - 1, y - origin_y + height - 1);
	vertexes[3].position = sf::Vector2f(x - origin_x, y - origin_y + height - 1);
}

void ms::GradientRect::draw(sf::RenderWindow &window) const
{
	window.draw(vertexes, 4, sf::Quads);
}