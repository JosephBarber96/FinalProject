#pragma once
class V2;
class jbShape;
class Intersection
{
public:
	Intersection();
	Intersection(V2* _point, jbShape* _shape);
	Intersection(float _x, float _y, jbShape* _shape);
	~Intersection();

	bool markedForDeletion;
	V2* point;
	jbShape* shapeIntersected;
};