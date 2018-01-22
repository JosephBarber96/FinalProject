#pragma once
#include "jbShape.h"

class jbSquare : public jbShape
{
public:
	jbSquare(float size);
	~jbSquare();

	void ResetShape(float size);
	bool Equal_To(jbShape* shape);
};