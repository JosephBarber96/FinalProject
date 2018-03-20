#define CITYLIB __declspec(dllexport)

class City;

extern "C"
{
	/* City */
	CITYLIB City* GenerateCity();

	/* Major roads */
	CITYLIB int GetMajorRoadCount(City* city);
	CITYLIB int GetMajorRoadSegmentCount(City* city, int index);
	CITYLIB void MajorRoadSegmentPos(City* city, int roadIndex, int segmentIndex, float &sx, float &sy, float &ex, float &ey);

	/* Minor roads */
	CITYLIB int GetMinorRoadCount(City* city);
	CITYLIB void MinorRoadPos(City* city, int index, float &sx, float &sy, float &ex, float &ey);

	/* Buildings */
	CITYLIB int MinorRoadBuildingCount(City* city, int index);
	CITYLIB void MinorRoadBuildingPos(City* city, int roadIndex, int buildingIndex, float &x, float &y);
	CITYLIB int BuildingVertCount(City* city, int roadIndex, int buildingIndex);
	CITYLIB void GetBuildingLine(City* city, int roadIndex, int buildingIndex, int lineIndex, float &sx, float &sy, float &ex, float &ey);

	/* Terrain */
	CITYLIB bool IsWater(City* city, int x, int y);
}