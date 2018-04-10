#include "Intersection.h"
#include "Vec2.h"
#include "jbShape.h"

Intersection::Intersection() {}

Intersection::Intersection(Vec2* _point, jbShape* _shape)
	:
	point(new Vec2(_point->x, _point->y)),
	shapeIntersected(_shape),
	markedForDeletion(false)
{}

Intersection::Intersection(float _x, float _y, jbShape* _shape)
	:
	point(new Vec2(_x, _y)),
	shapeIntersected(_shape),
	markedForDeletion(false)
{}

Intersection::~Intersection() {}