#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace sa
{

class Sequence
{
private:
	std::vector <double> data;
	double n, min, max, sum, average, normalDeviation;
public:
	bool loadFromFile(std::wstring fileName);
	void analyze();
	double getCount() const;
	double getMin() const;
	double getMax() const;
	double getAverage() const;
	double getNormalDeviation() const;
	double getSum() const;
	double getRangeCount() const;
	double getRangeWidth() const;
	double getDispersion() const;
	long size() const;
	double operator [](long i) const;
	void clear();

	friend std::wofstream &operator <<(std::wofstream &s, Sequence &seq);
	friend std::wifstream &operator >>(std::wifstream &s, Sequence &seq);
};

}