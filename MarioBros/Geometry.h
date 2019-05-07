#pragma once

struct Vector2 {
	float x;
	float y;

	Vector2() :x(0), y(0) {};
	Vector2(float inx, float iny) :x(inx), y(iny) {};

	void operator=(const Vector2& in) {
		x = in.x;
		y = in.y;
	}

	void operator+=(const Vector2& in) {
		x += in.x;
		y += in.y;
	}

	void operator-=(const Vector2& in) {
		x -= in.x;
		y -= in.y;
	}
};

Vector2 operator-(const Vector2& lv, const Vector2& rv);
Vector2 operator/(const Vector2& lv, const float f);
float GetDistance(const Vector2& lv, const Vector2& rv);

class Geometry
{
public:
	Geometry();
	~Geometry();
};

///サイズを表す構造体
struct Size {
	int width;
	int height;

	Size() : width(0), height(0) {}
	Size(int inx, int iny) : width(inx), height(iny) {}
};

///矩形を表現する構造体
struct Rect {
	Vector2 center;
	Size size;

	Rect() : center(0, 0), size(0, 0) {}
	Rect(int x, int y, int w, int h) : center(x, y), size(w, h) {}
	Rect(Vector2 &pos, Size &sz) : center(pos), size(sz) {}

	const int Left()const	{ return (center.x - size.width / 2); }
	const int Right()const	{ return (center.x + size.width / 2); }
	const int Top()const	{ return (center.y - size.height / 2); }
	const int Bottom()const { return (center.y + size.height / 2); }
	const int Width()const	{ return size.width; }
	const int Height()const { return size.height; }
};
