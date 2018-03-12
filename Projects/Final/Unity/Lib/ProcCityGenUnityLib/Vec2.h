#pragma once
class Vec2
{
public:
	Vec2();
	Vec2(float newX, float newY);
	~Vec2();

	float x, y;
	float Length();
	Vec2* Normalized();
	static float Length(Vec2 vec);

	// Operators
	bool operator== (Vec2 arg);
	bool operator!=(Vec2 arg);
	Vec2 operator- (Vec2 arg);

	// Static
	static float DistanceBetween(Vec2 left, Vec2 right);
	static float DegreeToRadians(float degree);
	static Vec2* AngleToVector(float degree);
	static float VectorToAngle(Vec2 vec);
	static float VectorToAngle(Vec2* vec);
};