#include "Geometry.h"

#include <math.h>

Geometry::Geometry()
{
}


Geometry::~Geometry()
{
}

Vector2 operator-(const Vector2 & lv, const Vector2 & rv)
{
	return Vector2(lv.x - rv.x, lv.y - rv.y);
}

Vector2 operator/(const Vector2 & lv, const float f)
{
	return Vector2(lv.x / f, lv.y / f);
}

float GetDistance(const Vector2 & lv, const Vector2 & rv)
{
	return sqrt((lv.x - rv.x) * (lv.x - rv.x) + (lv.y - rv.y) * (lv.y - rv.y));
}
