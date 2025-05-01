/* Green Ray window library */

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <windows.h>
#include "GreenRayInterface.h"

namespace gr  // This is the main Green Ray namespace
{

/* An SFML-dependent class which allows you to create and manage a window */
class Window
{
private:
	sf::RenderWindow sfWin;
	sf::Color backgroundColor;
	sf::Image icon;
	std::vector <Interactive*> config;
	std::vector <sf::Drawable*> sfmlObjects;
public:
	/* Conctructor
	w: width of the window in pixels
	h: height of the window in pixels
	t: title of the window */
	Window(long w, long h, sf::String t, sf::Uint32 style);

	/* Destructor */
	~Window();

	/* Does all the stuff such clearing,
	drawing all objects and displaying.
	Returns false if the window closed,
	else returns true. Notice that the
	sfml-objects are drawn earlier than
	the interface ones. */
	bool work();

	/* Sets background color
	of the window 
	c: color object */
	void setBackgroundColor(sf::Color c);

	/* Puts a pointer to an interface object
	into the window configuration buffer.
	Notice that the object must exist all
	time while it is being used. */
	Window &operator <<(Interactive &right);

	/* Deletes all interface
	stuff from window */
	void clearInterface();

	/* Deletes all sprites
	from the window */
	void clearSpriteBuffer();

	/* Returns a reference to
	an interface object
	i: index */
	Interactive &operator [](long i);

	/* Puts a pointer to an sfml drawable
	object into the window object buffer.
	Notice that the object must exist all
	time while it is being used. */
	Window &operator <<(sf::Drawable &right);

	/* Returns an sfml instance of the window */
	sf::RenderWindow &getSfmlWindow();

	/* Sets position of the window */
	void setPosition(long x, long y);

	/* Changes the size of the window */
	void setSize(long w, long h);

	/* Hides the console window */
	static void hideConsole();

	/* Shows the console window */
	static void showConsole();

	/* Sets the application icon */
	bool setIcon(sf::String path);

	/* Hides the window */
	void hide();

	/* Shows the window */
	void show();
};

}