/*
	Requires C++ Boost Library for 1.65s for windows
	https://dl.bintray.com/boostorg/release/1.65.1/binaries/
	(boost_1_65_1-msvc-10.0-32.exe)

	include directory: C:\local\boost_1_65_1
*/

#include "City.h"

int main()
{
	City city;
	city.Generate();
	city.Draw();
	return 0;
}