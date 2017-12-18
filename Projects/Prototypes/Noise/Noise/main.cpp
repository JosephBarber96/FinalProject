#include <array>

#include "FastNoise.h"

int main()
{
	FastNoise fn;
	fn.SetNoiseType(FastNoise::Perlin);

	const int size = 500;

	std::array<std::array<float, size>, size> popMap;

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			popMap[y][x] = fn.GetNoise(x, y);
		}
	}


	return 0;
}