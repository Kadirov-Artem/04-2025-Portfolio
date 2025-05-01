#include "IntCrd.h"

bool gr::operator <(const IntCrd &l, const IntCrd &r)
{
	return l.y < r.y || l.y == r.y && l.x < r.x;
}