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

	static jbShape* CreateShape(Shape shape);

	V2* position;
	std::vector<V2*> points;
	std::vector<Line*> lines;

protected:
	void GenerateLines();
};