#pragma once

namespace gr
{

class IntCrd
{
public:
	int x, y;
public:
	inline IntCrd(int ax, int ay): x(ax), y(ay) {}
	
	friend bool operator <(const IntCrd &l, const IntCrd &r);
};

}