#include <algorithm>
#include "Test.h"

TestDLL int Add(int a, int b)
{
	return (a + b);
}

TestDLL void Sort(int ia[], int size)
{
	std::sort(ia, ia + size);
}