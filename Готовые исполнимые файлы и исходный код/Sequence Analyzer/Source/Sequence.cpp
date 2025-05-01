#include "Sequence.h"

/* Sequence */

bool sa::Sequence::loadFromFile(std::wstring fileName)
{
	/* Очистить список значений */
	data.clear();

	/* Определить переменные и открыть поток */
	std::ifstream file;
	file.open(fileName);

	/* Если не удаётся открыть файл */
	if (!file)
	{
		return false;
	}
	/* Если файл открылся */
	else
	{
		sum = 0;

		/* Считать данные из файла */
		for (double val; file >> val;)
		{
			data.push_back(val);
			sum += val;
		}

		file.close();

		/* Расчёты */
		analyze();

		return true;
	}
}

void sa::Sequence::analyze()
{
	/* Расчёты */
	n = data.size();  // Кол-во элементов
	average = sum / n;  // Среднее арифметическое
	double squareDeviationsSum = 0;  // Сумма квадратов отклонений
	max = min = data[0];  // Сбросить макс. и мин. значения

	/* Вычислить сумму квадратов отклонений */
	for (std::vector <double>::iterator iter = data.begin(); iter != data.end(); iter++)
	{
		/* Нарастить сумму */
		squareDeviationsSum += (*iter - average) * (*iter - average);

		/* Обновить максимум/минимум */
		if (*iter > max)
		{
			max = *iter;
		}

		if (*iter < min)
		{
			min = *iter;
		}
	}

	/* Вычислить среднеквадратичное отклонение */
	normalDeviation = std::sqrt(squareDeviationsSum / (n - 1));
}

double sa::Sequence::getCount() const
{return n;}

double sa::Sequence::getMin() const
{return min;}

double sa::Sequence::getMax() const
{return max;}

double sa::Sequence::getAverage() const
{return average;}

double sa::Sequence::getNormalDeviation() const
{return normalDeviation;}

double sa::Sequence::getSum() const
{return sum;}

long sa::Sequence::size() const
{return data.size();}

double sa::Sequence::operator [](long i) const
{return data[i];}

double sa::Sequence::getRangeCount() const
{return round(sqrt(n));}

double sa::Sequence::getRangeWidth() const
{return (max - min) / round(sqrt(n));}

double sa::Sequence::getDispersion() const
{return normalDeviation * normalDeviation;}

void sa::Sequence::clear()
{
	n = 0;
	data.clear();
}

std::wofstream &sa::operator <<(std::wofstream &s, sa::Sequence &seq)
{
	for (std::vector <double>::iterator iter = seq.data.begin();
		 iter != seq.data.end(); iter++)
	{
		s << *iter << std::endl;
	}

	return s;
}

std::wifstream &sa::operator >>(std::wifstream &s, sa::Sequence &seq)
{
	/* Сначала удалить существующие значения */
	seq.data.clear();
	seq.sum = 0;

	/* Считать данные из потока */
	for (double val; s >> val;)
	{
		seq.data.push_back(val);
		seq.sum += val;
	}

	/* Проанализировать последовательность */
	seq.analyze();

	return s;
}