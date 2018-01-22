#pragma once
#include <vector>

class V2;
class jbShape;
class Intersection;
class shapeLine
{
public:
	shapeLine();
	shapeLine(V2* _start, V2* _end, jbShape* _parent);
	shapeLine(float sX, float sY, float eX, float eY, jbShape* _parent);
	~shapeLine();

	void AddIntersection(V2* point, jbShape* intersected);
	void AddIntersection(int x, int y, jbShape* intersected);

	void MarkForDeletion() { markedForDeletion = true; }
	bool IsMarkedForDeletion() const { return markedForDeletion; }

	std::vector<shapeLine*> returnLines();

	V2* start, *end;
	jbShape* parent;
	std::vector<Intersection*> intersections;

private:
	bool markedForDeletion;
};