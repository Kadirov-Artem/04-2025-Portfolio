#include "Table.h"
#include <algorithm>

ltb::Table::Table():
	min_columns(2000000000L),
	counter_name(L"")
{}

void ltb::Table::load_from_file(std::wstring file_name)
{
	std::ifstream stream(file_name);

	if (!stream)
	{
		throw Exception(L"Read", L"Невозможно открыть файл", 0, file_name.size());
	}

	src_data.clear();  // Удалить старую таблицу
	min_columns = 2000000000L;  // Сбросить мин. количество столбцов

	long line_count = 0;  // Счётчик строк файла
	while (true)  // Читать файл построчно
	{
		line_count++;
		std::string line;
		std::getline(stream, line, '\n');

		if (!stream)  // Если конец файла
		{
			stream.close();
			break;
		}

		std::istringstream mem_stream;
		mem_stream.str(line);
		std::vector <double> row;

		while (true)  // Из каждой строки прочитать числа
		{
			std::string str_value;
			mem_stream >> str_value;

			if (!mem_stream)  // Если конец потока
			{
				break;
			}
			
			if (!is_double(str_value))  // Если строка неверного формата
			{
				throw Exception(L"Read", L"Некорректное значение: \"" +
								str_to_wstr(str_value) + L"\"",	line_count, 0);
			}

			row.push_back(atof(str_value.c_str()));  // Поместить число в массив
		};

		src_data.push_back(row);  // Поместить массив в массив строк таблицы

		if (row.size() < min_columns)  // Обновить мин. знач. кол-ва столбцов
		{
			min_columns = row.size();
		}
	}
}

void ltb::Table::print_source() const
{
	for (long c = 0; c < src_data.size(); c++)
	{
		for (long r = 0; r < src_data[c].size(); r++)
		{
			std::cout << src_data[c][r] << '\t';
		}

		std::cout << std::endl;
	}
}

void ltb::Table::define_functions(std::wstring raw)
{
	functions.clear();  // Очистить список выражений
	vertical_bounds.clear();  // Очистить список вертикальных 
	precisions.clear();  // Очистить список идентификаторов точности значений выражений

	while (long bars_count = remove_vertical_bars(raw))  // Пока не закончились вертикальные черты
	{
		vertical_bounds.push_back(bars_count);  // Запомнить кол-во вертикальных черт

		if (raw.find('|') != -1)  // Если остались ещё черты
		{
			std::wstring expression = raw.substr(0, raw.find('|'));  // Получить выражение
			raw.erase(0, raw.find('|'));  // Удалить его из строки, оставив черту
			precisions.push_back(extract_precision(expression));  // Извлечь идентификатор точности
			functions.push_back(expression);  // Поместить выражение в вектор
		}
	}

	if (!functions.size())  // Если не было добавлено ни одной функции
	{
		throw Exception(L"EmptyFunctionList", L"Необходимо указать как минимум 1 функцию", 0, 0);
	}
}

void ltb::Table::define_variables(std::wstring raw)
{
	identificators.clear();  // Очистить список переменных
	long start_size = raw.size();  // Запомнить начальную длину строки

	while (true)
	{
		remove_spaces(raw);

		if (!raw.size())  // Если строка закончилась
		{
			break;
		}

		std::wstring name = extract_first_identificator(raw);  // Извлечь идентификатор

		if (!name.size())  // Если идентификатор не извлёкся - значит имеем запрещённый символ
		{
			std::wstring msg = L"Запрещённый первый символ идентификатора: \'";
			msg += raw[0];
			msg += '\'';
			throw Exception(L"VariableList", msg, start_size - raw.size(), 1);
		}

		// Если идентификатор используется для счётчика
		if (name == counter_name)
		{
			std::wstring msg = L"Переменная \'" + name + L"\' используется в качестве счётчика";
			throw Exception(L"VariableList", msg, start_size - raw.size() - name.size(),
							name.size());
		}

		// Если идентификатор ещё не встречался
		if (std::find(identificators.begin(), identificators.end(), name) ==
			identificators.end())
		{
			identificators.push_back(name);  // Добавить
		}
		else
		{
			std::wstring msg = L"Дублирующая переменная: \'" + name + L"\'";
			throw Exception(L"VariableList", msg, start_size - raw.size() - name.size(),
							name.size());
		}
	}
}

void ltb::Table::build()
{
	if (!src_data.size())  // Если таблица пуста
	{
		throw Exception(L"Build", L"Исходная таблица пуста", 0, 0);
	}

	// Если переменных больше, чем столбцов в самом коротком ряду таблицы
	if (identificators.size() > min_columns)  
	{
		throw Exception(L"Build", L"Кол-во переменных превышает кол-во столбцов", 0, 0);
	}

	output_data.clear();  // Очистить выходную таблицу

	for (long y = 0; y < src_data.size(); y++)  // Для каждой строки таблицы
	{
		try
		{
			for (long j = 0; j < functions.size(); j++)  // В каждой формуле
			{
				for (long i = 0; i < identificators.size(); i++)  // Инициализировать переменные
				{
					// Значениями из соответствующих столбцов
					functions[j].assign(identificators[i], src_data[y][i]); 
				}
	
				if (counter_name.size())  // Инициализировать переменную счётчика, если она указана
				{
					functions[j].assign(counter_name, y);  
				}
			}
	
			std::vector <double> output_row;  // Очередная строка выходной таблицы
	
			for (long f = 0; f < functions.size(); f++)  // Для каждой функции
			{
				// Вычислить значение и поместить его в соответствующий столбец таблицы
				output_row.push_back(functions[f].calculate());
			}
	
			output_data.push_back(output_row);  // Полученный ряд поместить в выходную таблицу
		}
		catch (ltb::Element::Exception error)
		{
			std::wstring msg = L"Строка " + ltb::to_string(y + 1) + L": " + error.termination_cause;
			throw Exception(L"Build", msg, 0, 0);
		}
	}
}

void ltb::Table::print_result() const
{
	for (long c = 0; c < output_data.size(); c++)
	{
		for (long r = 0; r < output_data[c].size(); r++)
		{
			std::cout << output_data[c][r] << '\t';
		}

		std::cout << std::endl;
	}
}

void ltb::Table::define_counter(std::wstring a_counter_name)
{
	// Если идентификатор не занят
	if (std::find(identificators.begin(), identificators.end(), a_counter_name) ==
		identificators.end())
	{
		counter_name = a_counter_name;  // Зарегистрировать его
	}
	else  // Если идентификатор занят
	{ 
		std::wstring msg = L"Нельзя определить счётчик \"" + a_counter_name +
						   L"\": переменная уже используется";
		throw Exception(L"CounterField", msg, 0, a_counter_name.size());
	}
}

void ltb::Table::save_as_tex(std::wstring file_name)
{
	std::ofstream stream(file_name);

	if (!stream)  // Если поток не открылся
	{
		throw Exception(L"Save", L"Невозможно открыть поток вывода", 0, file_name.size());
	}

	/* Сформировать заголовок таблицы */

	long columns_count = functions.size();
	std::wstring table_title;
	for (long i = 0; i < columns_count; i++)
	{
		table_title += repeat('|', vertical_bounds[i]) + L"c";
	}

	if (columns_count)
	{
		table_title += repeat('|', vertical_bounds[columns_count]);
		// Вертикальных границ всегда на одну больше, чем столбцов
	}

	stream << "\\begin{tabular}{" << wstr_to_str(table_title) << "}" << std::endl;
	stream << "\\hline" << std::endl;

	/* Распечатать содержимое таблицы */

	for (long y = 0; y < output_data.size(); y++)
	{
		// Распечатать все значения
		for(long x = 0; x < output_data[y].size(); x++)  
		{
			if (precisions[x] != -1)  // Если для значения указана точность
			{
				stream << std::fixed << std::setprecision(precisions[x]) << output_data[y][x] <<
						  std::defaultfloat;
			}
			else  // Если точность не указана
			{
				stream << output_data[y][x];
			}

			if (x != output_data[y].size() - 1)  // Если значение не последнее в строке
			{
				stream << " & ";
			}
		}

		stream << " \\\\ \\hline" << std::endl;  // Завершить строку
		/* Важно: кол-во столбцов таблицы равно кол-ву фукнций в заголовке.
		Это означает, что все строки таблицы обязательно имеют одинаковую длину,
		равную column_count */
	}

	/* Завершить таблицу */
	stream << "\\end{tabular}";
	stream.close();
}