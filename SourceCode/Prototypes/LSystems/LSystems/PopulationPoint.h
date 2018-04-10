#pragma once
#include "Vec2.h"

class PopulationPoint : public Vec2
{
public:
	PopulationPoint(float newX, float newY);
	~PopulationPoint();

	void setMarked() { marked = true; }
	bool getMarked() const { return marked; }

private:
	bool marked = false;
};

