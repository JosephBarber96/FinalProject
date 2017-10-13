#include <math.h>

#include "Utility.h"
#include "V2.h"

namespace Utility
{
	float Length(V2 vec)
	{
		return (float)sqrt((vec.x * vec.x) + (vec.y * vec.y));
	}

	float Length(V2* vec)
	{
		return (float)sqrt((vec->x * vec->x) + (vec->y * vec->y));
	}
}