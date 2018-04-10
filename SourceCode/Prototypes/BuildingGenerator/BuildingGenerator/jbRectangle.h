#pragma once
#include "jbShape.h"

class jbRectangle : public jbShape
{
public:
	jbRectangle();
	~jbRectangle();

	void ResetShape();
	bool Equal_To(jbShape* shape);
private:
	int orientation;
};