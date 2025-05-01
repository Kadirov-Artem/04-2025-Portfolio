#include <SFML/Graphics.hpp>
#include "Level.h"
#include "GlobalData.h"
#include "GameTick.h"
#include "Player.h"
#include "Debug.h"

int main()
{
	gr::setup();
	gr::Level level1("level1");

	sf::RenderWindow wnd(sf::VideoMode(gr::WND_WIDTH, gr::WND_HEIGHT), "Game", sf::Style::Default);
	wnd.setVerticalSyncEnabled(true);
	sf::Event event;

	while (wnd.isOpen())
	{
		while (wnd.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				wnd.close();
			}
			else if (event.type == sf::Event::LostFocus)
			{
				gr::game_tick.pause();
			}
			else if (event.type == sf::Event::GainedFocus)
			{
				gr::game_tick.resume();
			}
		}

		wnd.clear(sf::Color(135, 206, 235));
		
		gr::game_tick.update();
		gr::Entity::update_all();
		gr::debug.update();
		gr::cam.update();
		
		level1.render(wnd);
		gr::Entity::render_all(wnd);
		gr::debug.print(wnd);
		
		wnd.display();
	}
}