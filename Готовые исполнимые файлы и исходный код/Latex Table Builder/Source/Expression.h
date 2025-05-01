#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "str_functions.h"

namespace ltb
{


class Element;
struct Item;


class Expression  // Класс, для выражений
{
public:
	static const std::vector <std::wstring> PREDEFINED_FUNCTIONS;
	static const std::vector <Item> PREDEFINED_CONSTANTS;
private:
	const std::wstring raw_string;  // Выражение в строковом виде
	std::vector <Element*> expression;  // Массив элементов выражения
	std::vector <Item> variables;  // Список переменных с их значениями

	/* Принимает выражение s в виде строки.
	Извлекает из строки первый элемент и
	записывает его в объект элемента
	выражения по указателю e.
	В случае неправильного форматирования
	возбуждает исключение. Если входная строка
	пуста, записывает в указатель значение
	NULL и ничего не делает. */
	void extract(std::wstring &s, Element *&e);
public:
	/* Конструктор
	s - строка с выражением */
	Expression(std::wstring s);

	/* Деструктор */
	~Expression();

	/* Разбирает сырую строку и собирает вектор
	объектов-элементов выражения.
	В случае неправильного форматирования
	возбуждает исключение */
	void build();

	/* (Для отладки) Распечатывает
	выражение в консоли */
	void print() const;

	/* Назначает переменной новое значение */
	void assign(std::wstring name, double new_value);

	/* Принимает вектор выражения expr, разбирает
	его почленно и оставляет в нём только
	один элемент, являющийся значением выражения.
	Обрабатывает только элементы в диапазоне [left; right] */
	static void parse(std::vector <Element*> &expr, const long left, long right);

	/* Очищает список переменных. Если на переменные
	ссылалось выражение, то его необходимо перестроить. */
	void clear_variables();

	/* Возвращает значение выражения. В случае любой ошибки
	при обработке выражения возбуждает исключение */
	double calculate();

	/* (Для отладки) Распечатывает
	выражение в консоли */
	static void print(const std::vector <Element*> &expr);

	class Exception
	{
	public:
		std::wstring termination_cause, error_type;
		long position, length;
	public:
		inline Exception(std::wstring type, std::wstring cause, long pos, long len):
			error_type(type), termination_cause(cause), position(pos), length(len)
		{}

		inline void out() const
		{std::wcerr << termination_cause << std::endl;}
	};
};


class Element  // Базовый класс для элемента выражения
{
private:
public:
	static const short MAX_PRIORITY = 6;
	static const double PI;
	static const double E;

	/* Возвращает приоритет оператора.
	Чем больше число, тем выше приоритет.
	Если объект является числом, вернёт 0. */
	virtual short get_priority() const = 0;

	/* Принимает левый и правый аргументы и
	возвращает вычисленное значение. Если у оператора
	нет одного из соседей, то в качестве указателя
	на него передаётся NULL. Если объект не
	является оператором, возбуждает исключение. */
	virtual double operator ()(const Element *left, const Element *right) const = 0;

	/* Возвращает значение, если объект является числом.
	Вызывает исключение в противном случае */
	virtual double get_value() const = 0;

	/* Преобразует объект в строку */
	virtual operator std::wstring() const = 0;

	class Exception
	{
	public:
		std::wstring termination_cause;
	public:
		inline Exception(std::wstring cause): termination_cause(cause)
		{}

		inline void out() const
		{std::wcerr << termination_cause << std::endl;}
	};
};


class Variable: public Element
{
private:
	const double *value;
public:
	static const short PRIORITY = 0;

	inline Variable(double *pointer): value(pointer)
	{}

	inline short get_priority() const
	{return PRIORITY;}

	inline double operator ()(const Element *left, const Element *right) const
	{
		throw Exception(L"Нельзя вызвать объект класса <Переменная>");
		return 0;
	}

	inline double get_value() const
	{return *value;}

	inline operator std::wstring() const
	{
		std::wostringstream mem;
		mem << *value;
		return mem.str();
	}
};


class Number: public Element
{
private:
	const double value;
public:
	static const short PRIORITY = 0;

	inline Number(double a_value): value(a_value)
	{}
	
	inline short get_priority() const
	{return PRIORITY;}

	inline double operator ()(const Element *left, const Element *right) const
	{
		throw Exception(L"Нельзя вызвать объект класса <Число>");
		return 0;
	}

	inline double get_value() const
	{return value;}

	inline operator std::wstring() const
	{
		std::wostringstream mem;
		mem << value;
		return mem.str();
	}
};


class Operator: public Element
{
private:
public:

	inline double get_value() const
	{
		throw Exception(L"Нельзя извлечь значение из объекта класса <Оператор>");
		return 0;
	}
};


class Addition: public Operator
{
private:
public:
	static const short PRIORITY = 1;

	inline short get_priority() const
	{return PRIORITY;}

	inline double operator ()(const Element *left, const Element *right) const
	{
		if (left == NULL)  // Если слева нет значения, то оператор ведёт себя как унарный
		{
			return right->get_value();
		}

		if (right == NULL)  // Отсутствие правого аргумента недопустимо
		{
			throw Exception(L"Отсутствие правого аргумента оператора сложения недопустимо");
		}

		return left->get_value() + right->get_value();
	}

	inline operator std::wstring() const
	{return L"+";}
};


class Substraction: public Operator
{
private:
public:
	static const short PRIORITY = 1;

	inline short get_priority() const
	{return PRIORITY;}

	inline double operator ()(const Element *left, const Element *right) const
	{
		if (left == NULL || right == NULL)  // Отсутствие хотя бы одного аргумента недопустимо
		{
			throw Exception(L"Отсутствие аргументов оператора вычитания недопустимо");
		}

		return left->get_value() - right->get_value();
	}

	inline operator std::wstring() const
	{return L"-";}
};


class UnarySubstraction: public Substraction
{
private:
public:
	static const short PRIORITY = 4;

	inline short get_priority() const
	{return PRIORITY;}

	inline double operator ()(const Element *left, const Element *right) const
	{
		if (right == NULL)  // Отсутствие правого аргумента недопустимо
		{
			throw Exception(L"Отсутствие правого аргумента оператора унарного вычитания недопустимо");
		}

		return -right->get_value();
	}
};


class Multiplication: public Operator
{
private:
public:
	static const short PRIORITY = 2;

	inline short get_priority() const
	{return PRIORITY;}

	inline double operator ()(const Element *left, const Element *right) const
	{
		if (left == NULL || right == NULL)  // Отсутствие хотя бы одного аргумента недопустимо
		{
			throw Exception(L"Отсутствие аргументов бинарного оператора недопустимо");
		}

		return left->get_value() * right->get_value();
	}

	inline operator std::wstring() const
	{return L"*";}
};


class Division: public Operator
{
private:
public:
	static const short PRIORITY = 2;

	inline short get_priority() const
	{return PRIORITY;}

	inline double operator ()(const Element *left, const Element *right) const
	{
		if (left == NULL || right == NULL)  // Отсутствие хотя бы одного аргумента недопустимо
		{
			throw Exception(L"Отсутствие аргументов бинарного оператора недопустимо");
		}

		if (right->get_value() == 0)  // Деление на ноль недопустимо 
		{
			throw Exception(L"На ноль делить нельзя");
		}

		return left->get_value() / right->get_value();
	}

	inline operator std::wstring() const
	{return L"/";}
};


class Exponentiation: public Operator
{
private:
public:
	static const short PRIORITY = 3;

	inline short get_priority() const
	{return PRIORITY;}

	inline double operator ()(const Element *left, const Element *right) const
	{
		if (left == NULL || right == NULL)  // Отсутствие хотя бы одного аргумента недопустимо
		{
			throw Exception(L"Отсутствие аргументов бинарного оператора недопустимо");
		}

		if (left->get_value() < 0)  // Нельзя возвести отрицательное число в степень
		{
			throw Exception(L"Нельзя возвести в степень отрицательное число");
		}

		if (left->get_value() == 0 && right->get_value() < 0)  // Нельзя возвести ноль в отрицательную степень
		{
			throw Exception(L"Нельзя возвести ноль в отрицательную степень");
		}

		return std::pow(left->get_value(), right->get_value());
	}

	inline operator std::wstring() const
	{return L"^";}
};


class Parenthesis: public Element
{
private:
public:
	static const short PRIORITY = 6;

	inline short get_priority() const
	{return PRIORITY;}

	inline double operator ()(const Element *left, const Element *right) const
	{
		throw Exception(L"Нельзя вызвать объект класса <Скобка>");
		return 0;
	}
};


class LeftParenthesis: public Parenthesis
{
private:
public:
	inline double get_value() const
	{
		return -1;
	}

	inline operator std::wstring() const
	{return L"(";}
};


class RightParenthesis: public Parenthesis
{
private:
public:
	inline double get_value() const
	{
		return 1;
	}

	inline operator std::wstring() const
	{return L")";}
};


struct Item
{
	std::wstring name;
	double value;
};


class Function: public Element
{
private:
	const std::wstring function_name;
public:
	static const short PRIORITY = 5;

	inline Function(std::wstring name): function_name(name)
	{}

	inline short get_priority() const
	{return PRIORITY;}

	inline double operator ()(const Element *left, const Element *right) const
	{
		if (right == NULL)  // Отсутствие правого аргумента недопустимо
		{
			throw Exception(L"Отсутствие аргумента функции недопустимо");
		}

		double arg = right->get_value();

		if (function_name == L"sin")
		{
			return sin(arg);
		}
		else if (function_name == L"cos")
		{
			return cos(arg);
		}
		else if (function_name == L"tg")
		{
			return tan(arg);  // Увы, мы не можем проверить, не равен ли аргумент +-PI / 2
		}
		else if (function_name == L"ctg")
		{
			double result = tan(arg);
			if (result)
			{
				return 1 / result;
			}
			else
			{
				throw Exception(L"Недопустимый аргумент для функции ctg");
			}
		}
		else if (function_name == L"arcsin")
		{
			if (arg >= -1 && arg <= 1)
			{
				return asin(arg);
			}
			else
			{
				throw Exception(L"Недопустимый аргумент для функции arcsin");
			}
		}
		else if (function_name == L"arccos")
		{
			if (arg >= -1 && arg <= 1)
			{
				return acos(arg);
			}
			else
			{
				throw Exception(L"Недопустимый аргумент для функции arccos");
			}
		}
		else if (function_name == L"arctg")
		{
			return atan(arg);
		}
		else if (function_name == L"arcctg")
		{
			if (arg == 0)
			{
				return PI / 2;
			}
			else
			{
				return atan(1 / arg);
			}
		}
		else if (function_name == L"ln")
		{
			if (arg > 0)
			{
				return log(arg);
			}
			else
			{
				throw Exception(L"Недопустимый аргумент для функции ln");
			}
		}
		else if (function_name == L"lg")
		{
			if (arg > 0)
			{
				return log(arg) / log(10);
			}
			else
			{
				throw Exception(L"Недопустимый аргумент для функции log");
			}
		}
		else if (function_name == L"sqrt")
		{
			if (arg >= 0)
			{
				return sqrt(arg);
			}
			else
			{
				throw Exception(L"Недопустимый аргумент для функции sqrt");
			}
		}
		else
		{
			std::wstring msg = L"Неизвестная функция: \"" + function_name + L"\"";
			throw Exception(msg);
		}
	}

	double get_value() const
	{
		throw Exception(L"Нельзя извлечь значение из объекта класса <функция>");
	}

	operator std::wstring() const
	{
		return function_name;
	}
};


}