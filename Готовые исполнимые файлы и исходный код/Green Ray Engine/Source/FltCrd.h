#pragma once

namespace gr
{

class FltCrd
{
public:
	float x, y;
public:
	inline FltCrd(float ax, float ay): x(ax), y(ay) {}

	friend bool operator <(const FltCrd &l, const FltCrd &r);
};

}