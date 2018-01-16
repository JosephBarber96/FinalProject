#pragma once
class V2;
class jbShape;
class Line;
class Intersection
{
public:
	Intersection();
	Intersection(V2* _point, Line* _parent, Line* _lineintersected, jbShape* _shapeintersected);
	Intersection(float _x, float _y, Line* _parent, Line* _lineintersected, jbShape* _shapeintersected);
	~Intersection();

	bool markedForDeletion;
	V2* point;
	Line* parent;
	Line* lineIntersected;
	jbShape* shapeIntersected;
};