#define _USE_MATH_DEFINES
#include <math.h>

#include "Utility.h"

namespace Utility
{
	float DegreeToRadians(float degree)
	{
		return (float)degree * M_PI / 180;
	}
}