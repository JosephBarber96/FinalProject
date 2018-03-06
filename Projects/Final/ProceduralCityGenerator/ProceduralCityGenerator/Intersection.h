#pragma once
class Vec2;
class jbShape;
class Intersection
{
public:
	Intersection();
	Intersection(Vec2* _point, jbShape* _shape);
	Intersection(float _x, float _y, jbShape* _shape);
	~Intersection();

	void MarkForDeletion() { markedForDeletion = true; }
	bool IsMarkedForDeletion() const { return markedForDeletion; }

	Vec2* point;
	jbShape* shapeIntersected;
private:
	bool markedForDeletion;
};