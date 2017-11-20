#pragma once
class Vec2
{
public:
	Vec2(float newAngle);
	Vec2(float newX, float newY);
	~Vec2();

	static Vec2* AngleToVector(float degree);

	float getX() { return x; }
	float getY() { return y; }
	float Length();

	bool operator== (Vec2 vec);
	Vec2* operator* (float scale);
	Vec2* operator+ (Vec2 vec);
	Vec2* operator/ (float scale);

protected:
	float x, y;
};