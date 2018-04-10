#include "UnityLib.h"

#include "Vec2.h"
#include "City.h"
#include "RoadNetwork.h"
#include "MajorRoad.h"
#include "MinorRoad.h"
#include "BuildingLot.h"
#include "WaterData.h"
#include "FloorPlan.h"
#include "Line.h"
#include "shapeLine.h"
#include "DiamondSquare.h"

/* City */

CITYLIB City* GenerateCity(int citySize, float maxTerrainHeight, float percentWater)
{
	City* city = new City();
	city->Generate(citySize, maxTerrainHeight, percentWater);
	return city;
}

/* Major roads */

CITYLIB int GetMajorRoadCount(City* city)
{
	return city->GetRoadNetwork()->MajorRoadCount();
}

CITYLIB int GetMajorRoadSegmentCount(City* city, int index)
{
	return city->GetRoadNetwork()->GetMajorRoad(index)->segments.size();
}

CITYLIB void MajorRoadSegmentPos(City* city, int roadIndex, int segmentIndex, float &sx, float &sy, float &ex, float &ey)
{
	MajorRoad* mr = city->GetRoadNetwork()->GetMajorRoad(roadIndex);

	sx = mr->segments[segmentIndex]->Start()->x;
	sy = mr->segments[segmentIndex]->Start()->y;

	ex = mr->segments[segmentIndex]->End()->x;
	ey = mr->segments[segmentIndex]->End()->y;
}

/* Minor roads */

CITYLIB int GetMinorRoadCount(City* city)
{
	return city->GetRoadNetwork()->MinorRoadCount();
}

CITYLIB void MinorRoadPos(City* city, int index, float &sx, float &sy, float &ex, float &ey)
{
	MinorRoad mr = *city->GetRoadNetwork()->GetMinorRoad(index);

	sx = mr.Start()->x;
	sy = mr.Start()->y;

	ex = mr.End()->x;
	ey = mr.End()->y;
}

/* Buildings */

CITYLIB int MinorRoadBuildingCount(City* city, int index)
{
	return city->GetRoadNetwork()->GetMinorRoad(index)->lots.size() - 1;
}

CITYLIB void MinorRoadBuildingPos(City* city, int roadIndex, int buildingIndex, float &x, float &y)
{
	BuildingLot lot = *city->GetRoadNetwork()->GetMinorRoad(roadIndex)->lots[buildingIndex];

	x = lot.minX + (lot.GetWidth() / 2);
	y = lot.minY + (lot.GetHeight() / 2);
}

CITYLIB int BuildingVertCount(City* city, int roadIndex, int buildingIndex)
{
	return city->GetRoadNetwork()->GetMinorRoad(roadIndex)->lots[buildingIndex]->fp->perimeterLines.size();
}

CITYLIB void GetBuildingLine(City* city, int roadIndex, int buildingIndex, int lineIndex, float &sx, float &sy, float &ex, float &ey)
{
	FloorPlan* fp = city->GetRoadNetwork()->GetMinorRoad(roadIndex)->lots[buildingIndex]->fp;

	sx = fp->perimeterLines[lineIndex]->start->x;
	sy = fp->perimeterLines[lineIndex]->start->y;

	ex = fp->perimeterLines[lineIndex]->end->x;
	ey = fp->perimeterLines[lineIndex]->end->y;
}

/* Terrain */

CITYLIB int TerrainSize(City* city)
{
	return (city->CitySize());
}

CITYLIB float GetTerrainMaximumHeight(City* city)
{
	return (city->GetTerrain()->Highest() + fabsf(city->GetTerrain()->Lowest()));
}

CITYLIB float GetTerrainMinimumHeight(City* city)
{
	return (city->GetTerrain()->Lowest() + fabsf(city->GetTerrain()->Lowest()));
}

CITYLIB float GetTerrainHeightAt(City* city, int x, int y)
{
	// Return all height data with the absolute value of the lowest added on so that the lowest value we can receive is 0
	return (city->GetTerrain()->GetHeightAtPoint(x, y) + fabsf(city->GetTerrain()->Lowest()));
}

CITYLIB float GetWaterHeightPercent(City* city)
{
	return (city->GetWaterData()->WaterPercent());
}

CITYLIB float GetWaterLevel(City* city)
{
	return (city->GetWaterData()->WaterLevel() + fabsf(city->GetTerrain()->Lowest()));
}

CITYLIB bool IsWater(City* city, int x, int y)
{
	return (city->GetWaterData()->IsWater(x, y));
}