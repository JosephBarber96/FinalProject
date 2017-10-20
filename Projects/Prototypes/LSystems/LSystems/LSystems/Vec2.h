#pragma once
class Vec2
{
public:
	Vec2(float newAngle, float newX, float newY);
	~Vec2();

	float getAngle() { return angle; }
	float getX() { return x; }
	float getY() { return y; }

private:
	float angle, x, y;
};

