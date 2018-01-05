#pragma once
#include <vector>

class Vec3
{
public:
	Vec3() {}
	Vec3(float _x, float _y, float _z)
		:
		x(_x),
		y(_y),
		z(_z)
	{}
	~Vec3() {}

	float x, y, z;
};

class WaterData;
class RoadNode;
class DiamondSquare
{
public:
	DiamondSquare(int div, float siz, float hei);
	~DiamondSquare();

	void Generate();
	void Print();
	void CalcuateBoundaryPoints();
	void CreatePoints();
	std::vector<std::vector<RoadNode*>> CreatePointsAndPassBackRoadNodes(int offsetForRoadNodes, WaterData &wd);

	int Lowest() { return lowest; }
	int Highest() { return highest; }
	int getDivisions() { return divisions; }
	float getSize() const { return size; }
	float getHeight() { return height; }
	std::vector<Vec3> Verts() { return terrainVertices; }
	std::vector<std::vector<Vec3*>> Points() { return points; }

private:
	int divisions;	// The number of divisions along the terrain (5 vertices = 4 faces)
					// this number must be a power of 2: 2, 4, 8, 16, 32, 64, etc...
	int vertCount;

	int lowest, highest;	// These hold the lowest and highest values of the 
							// Generated terrain after calculating boundary points

	float size;		// The size of the terrain (e.g. 10x10)
	float height;	// The maximum height for our terrain
	std::vector<Vec3> terrainVertices;

	std::vector<std::vector<Vec3*>> points;

	void DiaSqu(int row, int col, int size, float heightOffset);
};