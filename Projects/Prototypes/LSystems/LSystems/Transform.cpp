#include "Transform.h"

Transform::Transform() {}

Transform::Transform(float angle, Vec2* pos)
	:
	rotation(angle),
	position(pos)
{}

Transform::~Transform() {}