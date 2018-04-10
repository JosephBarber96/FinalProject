#pragma once
#include <vector>

#include <SFML\Graphics.hpp>

class shapeLine;
class V2;
class jbShape
{
public:
	jbShape();
	~jbShape();

	void SetPosition(float _x, float _y);
	void SetPosition(V2 pos);
	void DrawSelf(sf::RenderWindow *window);

	float GetWidth();
	float GetHeight();
	bool PointWithin(float x, float y);
	bool PointWithin(V2* pos);

	//! In place of our bool operator== as this is an abstract class
	virtual bool Equal_To(jbShape* shape) = 0;

	//! Factory
	static jbShape* CreateShape(float size);

	float shapeSize;
	V2* position;
	std::vector<V2*> points;
	std::vector<shapeLine*> lines;

protected:
	virtual void ResetShape(float size) = 0;
	void GenerateLines();
};