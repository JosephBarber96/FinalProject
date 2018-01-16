#pragma once
#include <vector>
class V2;
class jbShape;
class Intersection;
class Line
{
public:
	Line();
	Line(V2* _start, V2* _end, jbShape* _parent);
	Line(float sX, float sY, float eX, float ey, jbShape* _parent);
	~Line();

	void AddIntersection(V2* point, jbShape* intersectedShape);
	void AddIntersection(int x, int y, jbShape* intersectedShape);

	bool markedForDeletion;

	std::vector<Line*> returnLines();

	V2* start, *end;
	jbShape* parent;
	std::vector<Intersection*> intersections;
};