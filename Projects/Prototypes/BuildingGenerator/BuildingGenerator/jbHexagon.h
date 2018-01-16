#pragma once
#include "jbShape.h"

class jbHexagon : public jbShape
{
public:
	jbHexagon();
	~jbHexagon();

	void ResetShape();
	bool Equal_To(jbShape* shape);
};