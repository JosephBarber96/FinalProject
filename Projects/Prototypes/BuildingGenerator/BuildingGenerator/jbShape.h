#pragma once
#include <vector>

#include <SFML\Graphics.hpp>

enum Shape
{
	// triangle,
	square,
	rectangle,
	pentagon,
	hexagon,
	SHAPE_MAX = hexagon
};

class Line;
class V2;
class jbShape
{
public:
	jbShape();
	jbShape(V2* pos);
	jbShape(V2* pos, std::vector<V2*> shapePoints);
	~jbShape();

	void SetPosition(int _x, int _y);
	void SetPosition(V2 pos);
	void DrawSelf(sf::RenderWindow *window);

	float getWidth();
	float getHeight();
	bool PointWithin(int x, int y);
	bool PointWithin(V2* pos);

	//! In place of our bool operator== as this is an abstract class
	virtual bool Equal_To(jbShape* shape) = 0;

	static jbShape* CreateShape(Shape shape);

	Shape type;
	V2* position;
	std::vector<V2*> points;
	std::vector<Line*> lines;

protected:
	virtual void ResetShape() = 0;
	void GenerateLines();
};