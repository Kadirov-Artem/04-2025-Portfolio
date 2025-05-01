#include "Application.h"

int main()
{
	sf::Font font;
	const std::string font_name = "arial.ttf";

	try
	{
		if(!font.loadFromFile(font_name))  // Загрузить шрифт
		{
			throw ltb::Application::Exception("Application error: unable to open \"" +
											  font_name + "\" font file");
		}

		ltb::Application app(font);  // Инициализировать приложение
		app.main_loop();  // Главный цикл
	}
	catch (ltb::Application::Exception error)
	{
		error.out();
	}

	return 0;
}