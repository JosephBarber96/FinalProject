#pragma once
class V2
{
public:
	V2();
	V2(float _x, float _y);
	~V2();

	float Length();
	static float Length(V2 vec);
	static float DistanceBetween(V2 left, V2 right);
	
	bool operator==(V2 v);
	bool operator!=(V2 v);

	float x, y;
};