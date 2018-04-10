#pragma once
#include <vector>

class shapeLine;
class Vec2;
class jbShape
{
public:
	jbShape();
	~jbShape();

	void SetPosition(float _x, float _y);
	void SetPosition(Vec2 pos);

	float GetWidth();
	float GetHeight();
	bool PointWithin(float x, float y);
	bool PointWithin(Vec2* pos);

	//! In place of our bool operator== as this is an abstract class
	virtual bool Equal_To(jbShape* shape) = 0;

	//! Factory
	static jbShape* CreateShape(float size);

	float shapeSize;
	Vec2* position;
	std::vector<Vec2*> points;
	std::vector<shapeLine*> lines;

protected:
	virtual void ResetShape(float size) = 0;
	void GenerateLines();
};