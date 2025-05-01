#pragma once
#include <string>
#include <sstream>

namespace ltb
{

class Exception
{
public:
	std::wstring termination_cause;
public:
	inline Exception(std::wstring cause): termination_cause(cause)
	{}
};

extern const std::wstring ALLOWED_NAMES;
extern const std::wstring ALLOWED_SYMBOLS;
extern const std::wstring ALLOWED_OPERATORS;
extern const std::wstring ALLOWED_NAME_BEGINNINGS;

/* Извлекает первый идентификатор, найденный в строке.
Если первый символ - не буква и не нижнее подчёркивание,
возвращает пустую строку. Найденный идентификатор
удаляется из строки. */
std::wstring extract_first_identificator(std::wstring &str);

/* Проверяет, является ли строка вещественным числом
s - строка */
bool is_double(std::wstring s);

/* Проверяет, является ли строка вещественным числом
s - строка */
bool is_double(std::string s);

/* Возвращает число в виде строки */
std::wstring to_string(double val);

/* Удаляет идущие подряд вертикальные черты в
начале строки. Возвращает их количество. */
long remove_vertical_bars(std::wstring &str);

/* Удаляет пробелы в начале строки */
void remove_spaces(std::wstring &str);

/* Возвращает строку из повторяющихся символов.
chr - символ, count - кол-во повторений. */
std::wstring repeat(wchar_t chr, long count);

/* Приводит обычную строку к широкой строке */
std::wstring str_to_wstr(std::string str);

/* Приводит широкую строку к обычной */
std::string wstr_to_str(std::wstring str);

/* Извлекает из строки целое число, следующее
за двоеточием и возвращает его в качестве значения.
Часть строки справа от двоеточия, включая двоеточие,
удаляется. В случае отсутствия двоеточия, возвращает
-1. В случае, если двоеточий в строке несколько
или не удаётся извлечь целочисленное значение, 
возбуждается исключение. */
short extract_precision(std::wstring &str);
}