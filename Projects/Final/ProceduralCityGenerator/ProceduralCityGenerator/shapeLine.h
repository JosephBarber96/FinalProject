#pragma once
#include <vector>

class Vec2;
class jbShape;
class Intersection;
class shapeLine
{
public:
	shapeLine();
	shapeLine(Vec2* _start, Vec2* _end, jbShape* _parent);
	shapeLine(float sX, float sY, float eX, float eY, jbShape* _parent);
	~shapeLine();

	void AddIntersection(Vec2* point, jbShape* intersected);
	void AddIntersection(int x, int y, jbShape* intersected);

	void MarkForDeletion() { markedForDeletion = true; }
	bool IsMarkedForDeletion() const { return markedForDeletion; }

	std::vector<shapeLine*> returnLines();

	Vec2* start, *end;
	jbShape* parent;
	std::vector<Intersection*> intersections;

private:
	bool markedForDeletion;
};