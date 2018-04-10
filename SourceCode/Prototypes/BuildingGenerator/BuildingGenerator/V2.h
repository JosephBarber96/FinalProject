#pragma once
class V2
{
public:
	V2();
	V2(float _x, float _y);
	~V2();

	bool operator==(V2 v);
	bool operator!=(V2 v);

	float x, y;
};