#include "GreenRayWindow.h"

/* Window */

gr::Window::Window(long width, long height, sf::String title, sf::Uint32 style):
	sfWin(sf::VideoMode(width, height), title, style),
	backgroundColor(255, 255, 255)
{}

gr::Window::~Window()
{
	sfWin.close();
	clearInterface();
}

bool gr::Window::work()
{
	/* Check if the window exists */
	if(!sfWin.isOpen())
		return false;

	sf::Event event;
	bool isTextEntered = false;

	/* Event stuff */
	while(sfWin.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
		{
			sfWin.close();
			return false;
		}

		if (event.type == sf::Event::TextEntered)
		{
			gr::TextField::setEnteredCharacter(event.text.unicode);
			isTextEntered = true;
		}
	}

	/* If text wasn't entered */
	if (!isTextEntered)
	{
		gr::TextField::setEnteredCharacter(-1);
	}

	/* Clear the window */
	sfWin.clear(backgroundColor);

	/* Render all sfml objects */
	for(std::vector <sf::Drawable*>::iterator iter = sfmlObjects.begin();
		iter != sfmlObjects.end(); iter++)
	{
		sfWin.draw(**iter);
	}

	/* Refresh all GUI components */
	for(std::vector <Interactive*>::iterator iter = config.begin();
		iter != config.end(); iter++)
	{
		(*iter)->refresh(sfWin);
	}

	/* Render all GUI components backwards */
	for(std::vector <Interactive*>::reverse_iterator iter = config.rbegin();
		iter != config.rend(); iter++)
	{
		(*iter)->render(sfWin);
	}

	sfWin.display();

	return true;
}

void gr::Window::setBackgroundColor(sf::Color abackgroundColor)
{backgroundColor = abackgroundColor;}

gr::Window &gr::Window::operator <<(gr::Interactive &right)
{
	config.push_back(&right);
	return *this;
}

void gr::Window::clearInterface()
{
	for(std::vector <Interactive*>::iterator iter = config.begin();
		iter != config.end(); iter++)
	{
		(*iter)->reset();
	}

	config.clear();
}

gr::Interactive &gr::Window::operator [](long index)
{return *config[index];}

gr::Window &gr::Window::operator <<(sf::Drawable &right)
{
	sfmlObjects.push_back(&right);
	return *this;
}

void gr::Window::hideConsole()
{
	HWND console = GetConsoleWindow();
	ShowWindow(console, HIDE_WINDOW);
}

void gr::Window::clearSpriteBuffer()
{sfmlObjects.clear();}

sf::RenderWindow &gr::Window::getSfmlWindow()
{return sfWin;}

void gr::Window::setPosition(long x, long y)
{sfWin.setPosition(sf::Vector2i(x, y));}

void gr::Window::setSize(long w, long h)
{
	sfWin.setSize(sf::Vector2u(w, h));
	sfWin.setView(sf::View(sf::FloatRect(0, 0, w, h)));
}

bool gr::Window::setIcon(sf::String path)
{
	if(!icon.loadFromFile(path))
	{
		return false;
	}

	sfWin.setIcon(32, 32, icon.getPixelsPtr());
	return true;
}

void gr::Window::showConsole()
{
	HWND console = GetConsoleWindow();
	ShowWindow(console, SHOW_OPENWINDOW);
}