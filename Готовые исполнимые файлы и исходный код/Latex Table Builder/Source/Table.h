#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Expression.h"

namespace ltb
{

class Table
{
private:
	std::vector < std::vector <double> > src_data, output_data;
	std::vector <Expression> functions;
	std::vector <std::wstring> identificators;
	std::vector <short> vertical_bounds;
	std::vector <short> precisions;
	std::wstring counter_name;
	long min_columns;
public:
	/* Конструктор
	f - объект шрифта SFML
	сs - размер шрифта */
	Table();

	/* Загружает таблицу из файла
	n - имя файла */
	void load_from_file(std::wstring n);

	/* Выводит исходную таблицу в консоль */
	void print_source() const;

	/* Выводит полученную таблицу в консоль */
	void print_result() const;

	/* Определяет функции преобразования для таблицы.
	Принимает строку raw, представляющую собой разделённые
	вертикальной чертой выражения, содержащие переменные. */
	void define_functions(std::wstring raw);

	/* Определяет имена переменных, принимающих значение
	соответствующих им столбцов таблицы. Принимает
	строку raw, содержащую уникальные идентификаторы */
	void define_variables(std::wstring raw);

	/* Определяет переменную-счётчик. Если переменная
	уже занята, возбуждает исключение. */
	void define_counter(std::wstring counter_name);

	/* Строит таблицу в соответствии с полученными данными */
	void build();

	/* Сохраняет таблицу в стиле LaTeX */
	void save_as_tex(std::wstring file_name);

	class Exception
	{
	public:
		const std::wstring termination_cause, error_type;
		const long position, length;
	public:
		inline Exception(std::wstring type, std::wstring cause, long pos, long len):
			error_type(type), termination_cause(cause), position(pos), length(len)
		{}

		inline void out() const
		{std::wcerr << termination_cause << std::endl;}
	};
};

}