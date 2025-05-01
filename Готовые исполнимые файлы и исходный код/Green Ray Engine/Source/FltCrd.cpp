#include "FltCrd.h"

bool gr::operator <(const FltCrd &l, const FltCrd &r)
{
	return l.y < r.y || l.y == r.y && l.x < r.x;
}