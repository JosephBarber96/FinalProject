#pragma once
#include "jbShape.h"

class jbPentagon : public jbShape
{
public:
	jbPentagon();
	~jbPentagon();

	void ResetShape();
	bool Equal_To(jbShape* shape);
};