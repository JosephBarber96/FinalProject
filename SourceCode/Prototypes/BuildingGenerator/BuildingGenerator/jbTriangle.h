#pragma once
#include "jbShape.h"

class jbTriangle : public jbShape
{
public:
	jbTriangle();
	~jbTriangle();

	void ResetShape();
	bool Equal_To(jbShape* shape);
};