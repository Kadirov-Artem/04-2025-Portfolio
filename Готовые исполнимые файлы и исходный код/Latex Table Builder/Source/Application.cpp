#include "Application.h"

const int ltb::Application::WIDTH = 400;
const int ltb::Application::HEIGHT = 150;
const std::string ltb::Application::VERSION = " 1.2 (pre-1)";
const std::string ltb::Application::WIN_LABEL = "LaTex Table Builder" + VERSION;

ltb::Application::Application(sf::Font &font):
	window(WIDTH, HEIGHT, WIN_LABEL, sf::Style::Close),
	source_file_textfield(10, 10, 180, 20, L"Исходный файл", font),
	output_file_textfield(210, 10, 180, 20, L"Выходной файл", font),
	variables_textfield(10, 40, 180, 20, L"Переменные", font),
	table_header_textfield(10, 70, 380, 20, L"Формат новой таблицы", font),
	counter_name_textfield(210, 40, 180, 20, L"Счётчик", font),
	status_bar_label(200, 130, 12, L"", font, gr::Label::Center),
	build_button(10, 100, 100, 20, L"Построить", font),
	table()
{
	gr::Window::hideConsole();  // Спрятать консоль
	//gr::Window::showConsole();  // Показать консоль

	if(!window.setIcon("LaTex Table Builder.png"))
	{
		throw Exception("Application error: unable to open \"LaTeX Table Builder.png\" "
						"icon file.");
	}

	// Загрузить интерфейс в окно
	window << source_file_textfield << variables_textfield << table_header_textfield <<
			  counter_name_textfield << build_button << status_bar_label << output_file_textfield;
}

void ltb::Application::main_loop()
{
	while(window.work())
	{
		try
		{
			if (build_button.is_triggered())  // Если кнопка запуска нажата
			{
				restart_clock();
				run();
				display_operation_time();	
			}
		}
		catch(ltb::Table::Exception error)
		{
			display_error(error.termination_cause);

			if (error.error_type == L"Read")
			{
				source_file_textfield.highlight(error.position, error.length, sf::Color(255, 0, 0, 100));
			}
			else if (error.error_type == L"Save")
			{
				output_file_textfield.highlight(error.position, error.length, sf::Color(255, 0, 0, 100));
			}
			else if (error.error_type == L"VariableList")
			{
				variables_textfield.highlight(error.position, error.length, sf::Color(255, 0, 0, 100));
			}
			else if (error.error_type == L"CounterField")
			{
				counter_name_textfield.highlight(error.position, error.length, sf::Color(255, 0, 0, 100));
			}
			else if (error.error_type == L"FunctionList")
			{
				table_header_textfield.highlight(error.position, error.length, sf::Color(255, 0, 0, 100));
			}
		}
		catch(ltb::Expression::Exception error)
		{
			display_error(error.termination_cause);
		}
		catch(ltb::Element::Exception error)
		{
			display_error(error.termination_cause);
		}
		catch(ltb::Exception error)
		{
			display_error(error.termination_cause);
		}
	}
}

void ltb::Application::run()
{
	source_file_textfield.resetHighlight();  // Сбросить выделения ошибок
	output_file_textfield.resetHighlight();
	variables_textfield.resetHighlight();
	counter_name_textfield.resetHighlight();
	table_header_textfield.resetHighlight();

	// Определить переменные, счётчик и функции
	table.define_variables(variables_textfield.getText());

	if (counter_name_textfield.getText().getSize())
	{
		table.define_counter(counter_name_textfield.getText());
	}
	else
	{
		table.define_counter(L"");
	}

	table.define_functions(table_header_textfield.getText());

	table.load_from_file(source_file_textfield.getText());  
	table.build();
	table.save_as_tex(output_file_textfield.getText());
}

void ltb::Application::display_error(std::wstring message)
{
	status_bar_label.setTextColor(sf::Color(255, 0, 0));
	status_bar_label.setText(message);
}

void ltb::Application::restart_clock()
{operation_clock.restart();}

void ltb::Application::display_operation_time()
{
	long time = operation_clock.getElapsedTime().asMicroseconds();
	status_bar_label.setTextColor(sf::Color(0, 255, 0));
	status_bar_label.setText(L"Затраченное время: " + to_wstring(time) + L" мкс");
}

std::wstring ltb::Application::to_wstring(double value)
{
	std::wostringstream mem;
	mem << value;
	return mem.str();
}