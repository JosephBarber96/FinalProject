#pragma once
class V2
{
public:
	V2();
	V2(int newX, int newY);
	~V2();

	int x, y;
	float Length();

	// Operators
	bool operator== (V2 arg);
	V2 operator- (V2 arg);

	// Static
	static float DistanceBetween(V2 left, V2 right);
};