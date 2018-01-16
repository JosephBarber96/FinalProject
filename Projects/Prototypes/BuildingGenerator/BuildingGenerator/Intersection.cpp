#include "Intersection.h"
#include "V2.h"
#include "jbShape.h"
#include "Line.h"

Intersection::Intersection() {}

Intersection::Intersection(V2* _point, Line* _parent, Line* _lineintersected, jbShape* _shapeintersected)
	:
	point(new V2(_point->x, _point->y)),
	parent(_parent),
	lineIntersected(_lineintersected),
	shapeIntersected(_shapeintersected),
	markedForDeletion(false)
{}

Intersection::Intersection(float _x, float _y, Line* _parent, Line* _lineintersected, jbShape* _shapeintersected)
	:
	point(new V2(_x, _y)),
	parent(_parent),
	lineIntersected(_lineintersected),
	shapeIntersected(_shapeintersected),
	markedForDeletion(false)
{}

Intersection::~Intersection() {}