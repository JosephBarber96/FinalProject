#include "Intersection.h"
#include "V2.h"
#include "jbShape.h"

Intersection::Intersection() {}

Intersection::Intersection(V2* _point, jbShape* _shape)
	:
	point(new V2(_point->x, _point->y)),
	shapeIntersected(_shape),
	markedForDeletion(false)
{}

Intersection::Intersection(float _x, float _y, jbShape* _shape)
	:
	point(new V2(_x, _y)),
	shapeIntersected(_shape),
	markedForDeletion(false)
{}

Intersection::~Intersection() {}