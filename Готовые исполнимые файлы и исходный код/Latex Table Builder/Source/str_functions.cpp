#include "str_functions.h"

/* Константы */

const std::wstring ltb::ALLOWED_NAMES = L"abcdefghijklmnopqrstuvwxyz_"
									    L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const std::wstring ltb::ALLOWED_SYMBOLS = L"0123456789.+-*/^()_ "
										  L"abcdefghijklmnopqrstuvwxyz"
										  L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::wstring ltb::ALLOWED_OPERATORS = L"+-*/^()";
const std::wstring ltb::ALLOWED_NAME_BEGINNINGS = L"abcdefghijklmnopqrstuvwxyz_"
												  L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* Функции */

std::wstring ltb::extract_first_identificator(std::wstring &str)
{
	if (str.size())  // Если строка не пуста
	{
		if (ALLOWED_NAME_BEGINNINGS.find(str[0]) == -1)
		{
			return L"";
		}

		long stop = str.find_first_not_of(ALLOWED_NAMES);
		std::wstring result = str.substr(0, stop);
		str.erase(0, stop);
		return result;
	}
	else
	{
		return L"";
	}
}

bool ltb::is_double(std::wstring s)
{
	std::wistringstream mem;
	double useless;
	mem.str(s + L" ");
	mem >> useless;
	return mem.good();
}

bool ltb::is_double(std::string s)
{
	std::istringstream mem;
	double useless;
	mem.str(s + " ");
	mem >> useless;
	return mem.good();
}

std::wstring ltb::to_string(double val)
{
	std::wostringstream mem;
	mem << val;
	return mem.str();
}

long ltb::remove_vertical_bars(std::wstring &str)
{
	remove_spaces(str);
	long count = 0;

	while (str.size() && str[0] == '|')
	{
		count++;
		str.erase(0, 1);
	}

	return count;
}

void ltb::remove_spaces(std::wstring &str)
{
	while (str.size() && str[0] == ' ')
	{
		str.erase(0, 1);
	}
}

std::wstring ltb::repeat(wchar_t chr, long count)
{
	std::wstring result;

	for (long i = 0; i < count; i++)
	{
		result += chr;
	}

	return result;
}

std::wstring ltb::str_to_wstr(std::string str)
{
	return std::wstring(str.begin(), str.end());
}

std::string ltb::wstr_to_str(std::wstring str)
{
	return std::string(str.begin(), str.end());
}

short ltb::extract_precision(std::wstring &str)
{
	if (str.find(':') == -1)  // Если нет двоеточий
	{
		return -1;
	}

	if (str.find(':') != str.rfind(':'))  // Если в строке более одного двоеточия
	{
		throw Exception(L"В формуле более одного двоеточия");
	}

	if (str.find(':') == str.size() - 1)  // Если двоеточие в конце строки
	{
		throw Exception(L"Указан пустой идентификатор точности");
	}

	std::wstring str_val = str.substr(str.find(':') + 1, -1);  // Получить часть строки справа от :
	str.erase(str.find(':'), -1);  // Двоеточие и всё, что справа от него, из исходной строки

	if (!is_double(str_val))  // Если строка не является вещественным числом
	{
		throw Exception(L"Некорректное значение точности");
	}

	std::string temp(str_val.begin(), str_val.end());
	return atoi(temp.c_str());
}