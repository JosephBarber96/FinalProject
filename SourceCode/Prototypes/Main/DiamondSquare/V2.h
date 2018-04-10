#pragma once
class V2
{
public:
	V2();
	V2(float newX, float newY);
	~V2();

	float x, y;
	float Length();
	V2* Normalized();
	static float Length(V2 vec);

	// Operators
	bool operator== (V2 arg);
	bool operator!=(V2 arg);
	V2 operator- (V2 arg);

	// Static
	static float DistanceBetween(V2 left, V2 right);
	static float DegreeToRadians(float degree);
	static V2* AngleToVector(float degree);
	static float VectorToAngle(V2* vec);
};

