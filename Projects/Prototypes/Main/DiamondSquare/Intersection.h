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

	void MarkForDeletion() { markedForDeletion = true; }
	bool IsMarkedForDeletion() const { return markedForDeletion; }

	V2* point;
	jbShape* shapeIntersected;
private:
	bool markedForDeletion;
};