#pragma once
#include "jbShape.h"

class jbSquare : public jbShape
{
public:
	jbSquare();
	~jbSquare();

	void ResetShape();
	bool Equal_To(jbShape* shape);
};