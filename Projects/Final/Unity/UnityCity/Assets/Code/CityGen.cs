using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices; //DLL Import

public class CityGen : MonoBehaviour
{
    System.IntPtr cityPtr;

    /* City generator */
    [DllImport("ProcCityGenUnityLib.dll")]
    static extern System.IntPtr GenerateCity();

    /* Major road value getters */
    [DllImport("ProcCityGenUnityLib.dll")]
    static extern int GetMajorRoadCount(System.IntPtr city);

    [DllImport("ProcCityGenUnityLib.dll")]
    static extern int GetMajorRoadSegmentCount(System.IntPtr city, int index);

    [DllImport("ProcCityGenUnityLib.dll")]
    static extern void MajorRoadSegmentPos(System.IntPtr city, int roadIndex, int segmentIndex, ref float sx, ref float sy, ref float ex, ref float ey);

    /* Minor road value getters */
    [DllImport("ProcCityGenUnityLib.dll")]
    static extern int GetMinorRoadCount(System.IntPtr city);

    [DllImport("ProcCityGenUnityLib.dll")]
    static extern void MinorRoadPos(System.IntPtr city, int index, ref float sx, ref float sy, ref float ex, ref float ey);

    /* Buildings */
    [DllImport("ProcCityGenUnityLib.dll")]
    static extern int MinorRoadBuildingCount(System.IntPtr city, int index);

    [DllImport("ProcCityGenUnityLib.dll")]
    static extern void MinorRoadBuildingPos(System.IntPtr city, int roadIndex, int buildingIndex, ref float x, ref float y);

    /* Terrain */
    [DllImport("ProcCityGenUnityLib.dll")]
    static extern bool IsWater(System.IntPtr city, int x, int y);

    /* Prefabs */
    [Header("Prefabs")]
    public GameObject prefab_minorRoad;
    public GameObject prefab_majorRoad;
    public GameObject prefab_building;
    public GameObject prefab_terrainGrass;
    public GameObject prefab_terrainWater;

    private float grassY = -0.2f;
    private float waterY = -0.1f;
    private float minorRoadY = 0f;
    private float majorRoadY = 0.1f;

    /* Data */
    int citySize = 512;

    void Start()
    {
        GenCity();
        SpawnMajorRoads();
        SpawnMinorRoads();
        SpawnBuildings();
        SpawnTerrain();
    }

    void GenCity()
    {
        // Generate the city, hold a pointer to it
        Debug.Log("Generating city...");
        cityPtr = GenerateCity();
    }

    /// <summary>
    /// Loops through and spawns each Major Road
    /// </summary>
    void SpawnMajorRoads()
    {
        int roadCount = GetMajorRoadCount(cityPtr);
        for (int i = 0; i < roadCount; i++)
        {
            int segmentCount = GetMajorRoadSegmentCount(cityPtr, i);
            for (int j = 0; j < segmentCount; j++)
            {
                float startX = 0, startY = 0, endX = 0, endY = 0;
                MajorRoadSegmentPos(cityPtr, i, j, ref startX, ref startY, ref endX, ref endY);

                Road road = Instantiate(prefab_majorRoad, new Vector3(0, majorRoadY, 0), Quaternion.identity).GetComponent<Road>();
                road.SetStart(startX, startY);
                road.SetEnd(endX, endY);
                road.Adjust();

            }
        }
    }

    /// <summary>
    /// Loops through and spawns each Minor road
    /// </summary>
    void SpawnMinorRoads()
    {
        int roadCount = GetMinorRoadCount(cityPtr);
        for (int i = 0; i < roadCount; i++)
        {
            float startX = 0, startY = 0, endX = 0, endY = 0;
            MinorRoadPos(cityPtr, i, ref startX, ref startY, ref endX, ref endY);

            Road road = Instantiate(prefab_minorRoad, new Vector3(0, minorRoadY, 0), Quaternion.identity).GetComponent<Road>();
            road.SetStart(startX, startY);
            road.SetEnd(endX, endY);
            road.Adjust();
        }
    }

    /// <summary>
    /// Loops through and spawns each building
    /// </summary>
    void SpawnBuildings()
    {
        int roadCount = GetMinorRoadCount(cityPtr);
        for (int i = 0; i < roadCount; i++)
        {
            int buildingCount = MinorRoadBuildingCount(cityPtr, i);

            for (int j = 0; j < buildingCount; j++)
            {
                float buildingX = 0, buildingY = 0;
                MinorRoadBuildingPos(cityPtr, i, j, ref buildingX, ref buildingY);

                Instantiate(prefab_building, new Vector3(buildingX, 1.5f, buildingY), Quaternion.identity);
            }

        }
    }

    /// <summary>
    /// Loops through terrain and spawns water/land
    /// </summary>
    void SpawnTerrain()
    {
        float scaleAmount = (citySize / 2) / 10;
        GameObject grass = Instantiate(prefab_terrainGrass, new Vector3(citySize / 2, grassY, citySize / 2), Quaternion.identity);
        grass.transform.localScale = new Vector3(citySize / 10, 1, citySize / 10);

        for (int x = 0; x < citySize; x++)
        {
            for (int y = 0; y < citySize; y++)
            {
                if (IsWater(cityPtr, x, y))
                    Instantiate(prefab_terrainWater, new Vector3(x, waterY, y), Quaternion.identity);
            }
        }
    }
}