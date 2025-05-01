#pragma once
#include "Table.h"
#include "GreenRayInterface.h"
#include "GreenRayWindow.h"

namespace ltb
{

class Application
{
private:
	static const int WIDTH;
	static const int HEIGHT;
	static const std::string VERSION;
	static const std::string WIN_LABEL;

	sf::Font font;
	gr::Window window;

	gr::TextField source_file_textfield;
	gr::TextField output_file_textfield;
	gr::TextField variables_textfield;
	gr::TextField table_header_textfield;
	gr::TextField counter_name_textfield;
	gr::Label status_bar_label;
	gr::Button build_button;

	ltb::Table table;
	sf::Clock operation_clock;
public:
	Application(sf::Font &font);
	void main_loop();
	void run();
	void display_error(std::wstring message);
	void restart_clock();
	void display_operation_time();
	std::wstring to_wstring(double value);

	class Exception
	{
	public:
		std::string termination_cause;
	public:
		inline Exception(std::string cause):
			termination_cause(cause)
		{}

		inline void out() const
		{
			std::ofstream stream("CRASH REPORT.txt");
			stream << WIN_LABEL << " CRASH REPORT:" << std::endl;
			stream << termination_cause << std::endl;
			stream.close();
		}
	};
};

}