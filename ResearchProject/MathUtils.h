#pragma once
#include "structs.h"

inline auto DistanceSquared(const Point2f& v1, const Point2f& v2)
{
	return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
}