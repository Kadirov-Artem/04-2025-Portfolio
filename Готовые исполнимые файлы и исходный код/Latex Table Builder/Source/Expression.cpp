#include "Expression.h"

/* Expression */

const std::vector <std::wstring> ltb::Expression::PREDEFINED_FUNCTIONS =
{
	L"sin",
	L"cos",
	L"tg",
	L"ctg",
	L"arcsin",
	L"arccos",
	L"arctg",
	L"arcctg",
	L"ln",
	L"lg",
	L"sqrt"
};

const std::vector <ltb::Item> ltb::Expression::PREDEFINED_CONSTANTS =
{
	{L"pi", 3.14159265358979323846},
	{L"e", 2.71828182845904523536}
};

ltb::Expression::Expression(std::wstring s):
	raw_string(s)
{}

ltb::Expression::~Expression()
{
	// Освободить память
	for (long i = 0; i < expression.size(); i++)
	{
		delete expression[i];
	}
}

void ltb::Expression::extract(std::wstring &stream, Element *&element)
{
	static bool is_previous_element_numeric = false;  // Флаг: предыдущий элемент - число

	while (stream.size() && stream[0] == ' ')  // Стереть пробелы в начале строки
	{
		stream.erase(0, 1);
	}

	if (stream.size() == 0)  // Если строка пуста
	{
		element = NULL;
		is_previous_element_numeric = false;
		return;
	}

	if (stream[stream.size() - 1] != ' ')  // Если в конце нет пробела
	{
		stream += ' ';
	}

	char first = stream[0];
	
	if (first >= '0' && first <= '9' || first == '.')  // Если в начале находится число
	{
		std::wistringstream mem;
		double value;
		mem.str(stream);  // Поместить строку в поток
		mem >> value;  // Извлечь действительное число из потока
		std::getline(mem, stream);  // Остаток строки поместить в stream

		element = new Number(value);  // Записать по указателю числовой объект

		is_previous_element_numeric = true;
	}
	else if (ALLOWED_OPERATORS.find(first) != -1)  // Если это оператор
	{
		switch (first)
		{
		case '+':
			element = new Addition();
			break;
		case '-':
			if (is_previous_element_numeric)  // Если слева есть число
			{
				element = new Substraction();  // То минус бинарный
			}
			else  // Если же слева числа нет
			{
				element = new UnarySubstraction();  // То минус унарный
			}
			break;
		case '*':
			element = new Multiplication();
			break;
		case '/':
			element = new Division();
			break;
		case '^':
			element = new Exponentiation();
			break;
		case '(':
			element = new LeftParenthesis();
			break;
		case ')':
			element = new RightParenthesis();
			break;
		}

		stream.erase(0, 1);  // Стереть обработанный оператор
		is_previous_element_numeric = false;  // Обработанный элемент не является числом
	}
	else  // Если это первый символ идентификатора
	{
		/* Порядок поиска идентификаторов:
		   1. Переменные, определённые пользователем
		   2. Предопределённые константы
		   3. Предопределённые функции
		*/

		std::wstring name = extract_first_identificator(stream);  // Извлечь идентификатор
		
		bool is_variable_found = false;
		for (long i = 0; i < variables.size(); i++)
		{
			if (variables[i].name == name)  // Если переменная найдена в списке
			{
				is_variable_found = true;
				// Передать новому объекту адрес, по которому находится значение
				element = new Variable(&variables[i].value);
				break;
			}
		}

		if (!is_variable_found)  // Если переменная не найдена, искать константы
		{
			bool is_constant_found = false;
			for (long i = 0; i < PREDEFINED_CONSTANTS.size(); i++)
			{
				if (PREDEFINED_CONSTANTS[i].name == name)  // Если константа найдена
				{
					is_constant_found = true;
					// Создать объект класса "Число"
					element = new Number(PREDEFINED_CONSTANTS[i].value);
					break;
				}
			}

			if (!is_constant_found)  // Если константа не найдена, искать функции
			{
				if (std::find(PREDEFINED_FUNCTIONS.begin(), PREDEFINED_FUNCTIONS.end(), name) !=
					PREDEFINED_FUNCTIONS.end())
				{
					element = new Function(name);  // Задать объект функции
				}
				else  // Если и функция не найдена
				{
					std::wstring msg = L"Неизвестный идентификатор: \"" + name + L"\"";
					throw Exception(L"FunctionList", msg, 0, name.size());
				}
			}
		}

		is_previous_element_numeric = true;  // Обработанный элемент является числом
	}
}

void ltb::Expression::build()
{
	// Попытаться найти запрещённый символ
	long prohibited_char_pos = raw_string.find_first_not_of(ALLOWED_SYMBOLS);

	if (prohibited_char_pos != -1)  // Если найден запрещённый символ
	{
		std::wstring msg = L"Запрещённый символ: \'";
		msg += raw_string[prohibited_char_pos];
		msg += '\'';
		throw Exception(L"FunctionList", msg, prohibited_char_pos, 1);
	}

	std::wstring stream = raw_string;  // Скопировать строку, чтобы не потерять её

	for (long i = 0; i < expression.size(); i++)  // Очистить выражение
	{
		delete expression[i];
	}

	expression.clear();

	while (stream.size())  // Разбирать выражение по членам, пока строка не опустеет
	{
		//std::cout << stream << std::endl;
		Element *new_element;  // Указатель на новый член выражения

		try
		{
			// Извлечь член из выражения и записать адрес нового объекта в указатель
			extract(stream, new_element);  // Указатель передаётся по ссылке
		}
		catch (Exception error)
		{
			throw Exception(error.error_type, error.termination_cause,
							raw_string.size() - stream.size() - error.length + 1, error.length);
		}
		
		if (new_element)  // Если указатель не пуст
		{
			expression.push_back(new_element);  // Поместить указатель в вектор для выражения
		}
	}

	if (!expression.size())  // Если выражение осталось пустым
	{
		throw Exception(L"FunctionList", L"Указано пустое выражение", 0, 0);
	}
}

void ltb::Expression::print() const
{
	for (long i = 0; i < expression.size(); i++)
	{
		std::wcout << expression[i]->operator std::wstring() << ' ';
	}

	std::cout << std::endl;
}

void ltb::Expression::assign(std::wstring name, double new_value)
{
	if (name.find_first_not_of(ALLOWED_NAMES) != -1)  // Если имя переменной не является разрешённым
	{
		std::wstring msg = L"\"" + name + L"\" не является разрешённым идентификатором";
		throw Exception(L"CounterField", msg, 0, name.size());
	}

	bool is_variable_found = false;

	for (long i = 0; i < variables.size(); i++)
	{
		if (variables[i].name == name)  // Если переменная найдена в списке
		{
			is_variable_found = true;
			variables[i].value = new_value;  // Обновить её значение
			break;
		}
	}

	if (!is_variable_found)  // Если переменной ещё нет в списке
	{
		variables.push_back({name, new_value});  // Добавить её в список
	}
}

void ltb::Expression::parse(std::vector <Element*> &expr, const long left, long right)
{
	bool is_parenthesis_found;

	do  // Пока не закончились скобки
	{
		is_parenthesis_found = false;

		for (long i = left; i <= right; i++)
		{
			if (expr[i]->get_priority() == ltb::Parenthesis::PRIORITY)  // Если найдена скобка
			{
				is_parenthesis_found = true;
				break;
			}
		}

		if (is_parenthesis_found)  // Если скобки есть
		{
			long level = 0;  // Уровень вложенности
			long lPar = 0, rPar = 0;  // Индексы искомых скобок

			for (long i = left; i <= right; i++)  // Найти первую внешнюю пару скобок
			{
				if (expr[i]->get_priority() == ltb::Parenthesis::PRIORITY)
				{
					if (expr[i]->get_value() < 0)  // Если скобка левая
					{
						if (level == 0)  // Если это самая первая открывающая скобка
						{
							lPar = i;  // Запомнить её позицию
						}

						level++;
					}
					else  // Если скобка правая
					{
						level--;

						if (level == 0)  // Если это внешняя закрывающая скобка
						{
							rPar = i;  // Запомнить её позицию
							break;
						}
					}

					if (level < 0)  // Если лишняя закрывающая скобка
					{
						throw Exception(L"Build", L"Лишняя закрывающая скобка", left + i, 1);
					}
				}
			}
			
			if (level != 0)  // Если соответствующая закрывающая скобка так и не найдена
			{
				throw Exception(L"Build", L"Не найдена закрыающая скобка", right, 1);
			}
				
			long old_length = expr.size();  // Запомнить старую длину вектора
			parse(expr, lPar + 1, rPar - 1);  // Рекурсивный вызов: обработка содержимого скобок
			long delta = old_length - expr.size();  // Изменение длины списка
			right -= delta;  // Сдвиг правой границы за счёт укорачивания списка
			expr.erase(expr.begin() + rPar - delta);  // Удалить правую скобку
			expr.erase(expr.begin() + lPar);  // Удалить левую скобку
			right -= 2;  // Сдвиг правой границы за счёт удаления пары скобок
		}
	}
	while (is_parenthesis_found);

	if (left == right)  // Если в выражении остался только один элемент
	{
		return;
	}
	
	for (int priority = ltb::Element::MAX_PRIORITY; priority > 0; priority--)  // Искать операторы в порядке приоритета
	{
		for (long i = left; i <= right; i++)
		{
			if (expr[i]->get_priority() == priority)  // Если оператор имеет нужный нам приоритет
			{
				if (priority == ltb::UnarySubstraction::PRIORITY)  // Если найденный оператор является унарным
				{
					if (i == right)  // Если он крайний справа
					{
						throw Exception(L"Build",
										L"Унарные операторы не разрешены в конце выражения",
										left + i, 1);
					}
				}

				if (priority == ltb::Function::PRIORITY)  // Если найденный объект является функцией
				{
					if (i == right)  // Если он крайний справа
					{
						throw Exception(L"Build",
										L"Имена функций не разрешены в конце выражения",
										left + i, 1);
					}
				}

				// Если найденный объект - крайний слева или является унарным оператором или функцией
				if (i == left || priority == ltb::UnarySubstraction::PRIORITY ||
					priority == ltb::Function::PRIORITY)
				{
					double result = (*expr[i])(NULL, expr[i + 1]);  // Получить результат операции
					expr.erase(expr.begin() + i, expr.begin() + i + 2);  // Удалить оператор и его правый аргумент
					expr.insert(expr.begin() + i, new Number(result));  // На их место вставить вычисленное значение
					right -= 1;  // Сдвиг правой границы за счёт укорачивания списка
					i--;  // Не сдвигать индекс: на этом месте теперь новый объект
				}
				else if (i == right)  // Если найденный оператор - крайний справа
				{
					double result = (*expr[i])(expr[i - 1], NULL);  // Получить результат операции
					expr.erase(expr.begin() + i - 1, expr.begin() + i + 1);  // Удалить оператор и его левый аргумент
					expr.insert(expr.begin() + i - 1, new Number(result));  // На их место вставить вычисленное значение
					right -= 1;  // Сдвиг правой границы за счёт укорачивания списка
					i--;  // Не сдвигать индекс: на этом месте теперь новый объект
				}
				else  // Если найденный оператор не является крайним в выражении
				{
					double result = (*expr[i])(expr[i - 1], expr[i + 1]);  // Получить результат операции
					expr.erase(expr.begin() + i - 1, expr.begin() + i + 2);  // Удалить оператор с его аргументами
					expr.insert(expr.begin() + i - 1, new Number(result));  // На их место вставить вычисленное значение
					right -= 2;  // Сдвиг правой границы за счёт укорачивания списка
					i--;  // Не сдвигать индекс: на этом месте теперь новый объект
				}
			}
		}

		if (left == right)  // Если в выражении остался только один элемент
		{
			return;
		}
	}
}

double ltb::Expression::calculate()
{
	build();  // Построить выражение
	parse(expression, 0, expression.size() - 1);  // Разобрать выражение

	if (expression.size() != 1)  // Если в списке не один элемент
	{
		throw Exception(L"Build", L"Невозможно определить значение выражения", 0, 0);
	}

	return expression[0]->get_value();  // Вернуть вычисленное значение
}

void ltb::Expression::print(const std::vector <Element*> &expr)
{
	for (long i = 0; i < expr.size(); i++)
	{
		std::wcout << expr[i]->operator std::wstring() << ' ';
	}

	std::cout << std::endl;
}

void ltb::Expression::clear_variables()
{variables.clear();}

/* Element */

const double ltb::Element::PI = 3.14159265358979323846;
const double ltb::Element::E = 2.71828182845904523536;