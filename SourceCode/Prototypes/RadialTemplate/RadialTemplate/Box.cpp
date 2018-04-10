#include "Box.h"

Box::Box() {}

Box::Box(Vec2* tl, Vec2* tr, Vec2* bl, Vec2* br)
	:
	topLeft(tl),
	topRight(tr),
	bottomLeft(bl),
	bottomRight(br)
{}

Box::~Box() {}