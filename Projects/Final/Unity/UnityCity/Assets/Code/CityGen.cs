using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices; //DLL Import

public class CityGen : MonoBehaviour
{
    private int citySize;

    [Header("Terrain height")]
    [SerializeField]
    private float terrainHeight;

    [Header("Water level (percent)")]
    [SerializeField]
    private float waterLevelInPercent;

    [Header("Building height")]
    [SerializeField]
    private float buildingHeight;

    /* An IntPtr for holding the memory address of the C++ City object */
    System.IntPtr cityPtr;

    /* City generator */
    [DllImport("ProcCityGenUnityLib.dll")]
    static extern System.IntPtr GenerateCity(int citySize, float maxTerrainHeight, float percentWater);

    /* Major road value getters */
    [DllImport("ProcCityGenUnityLib.dll")]
    static extern int GetMajorRoadCount(System.IntPtr city);

    [DllImport("ProcCityGenUnityLib.dll")]
    static extern int GetMajorRoadSegmentCount(System.IntPtr city, int index);

    [DllImport("ProcCityGenUnityLib.dll")]
    static extern void MajorRoadSegmentPos(System.IntPtr city, int roadIndex, int segmentIndex, 
        ref float sx, ref float sy, ref float ex, ref float ey);

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

    [DllImport("ProcCityGenUnityLib.dll")]
    static extern int BuildingVertCount(System.IntPtr city, int roadIndex, int buildingIndex);

    [DllImport("ProcCityGenUnityLib.dll")]
    static extern int GetBuildingLine(System.IntPtr city, int roadIndex, int buildingIndex, int lineIndex,
        ref float sx, ref float sy, ref float ex, ref float ey);

    /* Terrain */
    [DllImport("ProcCityGenUnityLib.dll")]
    static extern int TerrainSize(System.IntPtr city);

    [DllImport("ProcCityGenUnityLib.dll")]
    static extern float GetTerrainMaximumHeight(System.IntPtr city);

    [DllImport("ProcCityGenUnityLib.dll")]
    static extern float GetTerrainMinimumHeight(System.IntPtr city);

    [DllImport("ProcCityGenUnityLib.dll")]
    static extern float GetTerrainHeightAt(System.IntPtr city, int x, int y);

    [DllImport("ProcCityGenUnityLib.dll")]
    static extern float GetWaterHeightPercent(System.IntPtr city);

    [DllImport("ProcCityGenUnityLib.dll")]
    static extern float GetWaterLevel(System.IntPtr city);

    [DllImport("ProcCityGenUnityLib.dll")]
    static extern bool IsWater(System.IntPtr city, int x, int y);

    /* Prefabs */
    [Header("Prefabs")]
    public GameObject prefab_minorRoad;
    public GameObject prefab_majorRoad;
    public GameObject prefab_building;
    public GameObject prefab_terrainGrass;
    public GameObject prefab_terrainWater;
    public GameObject prefab_wall;
    public GameObject prefab_terrain;
    public Material[] buildingMaterials;

    void Start()
    {
        // Generate the C++ City object
        GenCity();

        // Spawn roads of the city
        // SpawnMajorRoads();
        // SpawnMinorRoadsAndBuildings();

        // Spawn the terrain
        SpawnTerrain();
    }

    /// <summary>
    /// Generates the C++ City object, holds 
    /// a pointer to it in memory
    /// </summary>
    void GenCity()
    {
        Debug.Log("Generating city...");

        citySize = 512;

        // Generate the city, hold a pointer to it
        // Pass through city size, terrain height, water level
        cityPtr = GenerateCity(citySize, terrainHeight, waterLevelInPercent);
    }

    /// <summary>
    /// Loops through and spawns each Major Road
    /// </summary>
    void SpawnMajorRoads()
    {
        // Create a GameObject to hold the Major Roads
        GameObject majorRoads = new GameObject("MajorRoads");

        // For each major road
        int roadCount = GetMajorRoadCount(cityPtr);
        for (int i = 0; i < roadCount; i++)
        {
            // Create a GameObject to hold the segments of this Major Road
            GameObject majorRoad = new GameObject(string.Format("Major_Road_{0}", i));
            majorRoad.transform.SetParent(majorRoads.transform);

            // For each segment of this major road
            int segmentCount = GetMajorRoadSegmentCount(cityPtr, i);
            for (int j = 0; j < segmentCount; j++)
            {
                // Find the start and end position of this segment
                float startX = 0, startY = 0, endX = 0, endY = 0;
                MajorRoadSegmentPos(cityPtr, i, j, ref startX, ref startY, ref endX, ref endY);

                // Instantiate and place the road
                Road road = Instantiate(prefab_majorRoad, new Vector3(0, 1, 0), Quaternion.identity).GetComponent<Road>();
                road.gameObject.name = string.Format("Major_Road_Segment_{0}", j);
                road.SetStart(startX, startY);
                road.SetEnd(endX, endY);
                road.Adjust();

                // Parent in hierarchy
                road.gameObject.transform.SetParent(majorRoad.transform);
            }
        }
    }

    /// <summary>
    /// Loops through and spawns each Minor road
    /// </summary>
    void SpawnMinorRoadsAndBuildings()
    {
        // Create a GameObject to hold the MinorRoads
        GameObject minorRoads = new GameObject("MinorRoads");

        // For each minor road
        int roadCount = GetMinorRoadCount(cityPtr);
        for (int i = 0; i < roadCount; i++)
        {
            // Create a GameObject to hold the Road and the Buildings of this road
            GameObject minorRoad = new GameObject(string.Format("Minor_Road_{0}", i));
            minorRoad.transform.SetParent(minorRoads.transform);

            // Get the start and end points of this road
            float roadSX = 0, roadSY = 0, roadEX = 0, roadEY = 0;
            MinorRoadPos(cityPtr, i, ref roadSX, ref roadSY, ref roadEX, ref roadEY);

            // Instantiate the road
            Road road = Instantiate(prefab_minorRoad, new Vector3(0, 1, 0), Quaternion.identity).GetComponent<Road>();
            road.gameObject.name = string.Format("Road");

            // Set its position
            road.SetStart(roadSX, roadSY);
            road.SetEnd(roadEX, roadEY);
            road.Adjust();

            // Make it a child in the hierarchy
            road.transform.SetParent(minorRoad.transform);

            /* Buildings... */
            // Find out how many buildings this road has
            int buildingCount = MinorRoadBuildingCount(cityPtr, i);
            for (int j = 0; j < buildingCount; j++)
            {
                // Get the building (x, y) position
                float buildingX = 0, buildingY = 0;
                MinorRoadBuildingPos(cityPtr, i, j, ref buildingX, ref buildingY);

                // Get number of lines of the building
                int lineCount = BuildingVertCount(cityPtr, i, j);

                // Create a game object to hold this building
                GameObject building = new GameObject("Building");

                // What material will this building use
                int buildingMaterialIndexer = (int)Random.Range(0, buildingMaterials.Length);

                // For each edge of the building
                for (int k = 0; k < lineCount; k++)
                {
                    // Get the line
                    float sX = 0, sY = 0, eX = 0, eY = 0;
                    GetBuildingLine(cityPtr, i, j, k, ref sX, ref sY, ref eX, ref eY);

                    // Instantiate
                    Wall wall = Instantiate(prefab_wall, new Vector3(0, 1, 0), Quaternion.identity).GetComponent<Wall>();
                    wall.SetStart(sX, sY);
                    wall.SetEnd(eX, eY);
                    wall.Adjust();

                    // Position and extrude the wall (Y axis for both)
                    wall.transform.localScale = new Vector3(wall.transform.localScale.x, buildingHeight, wall.transform.localScale.z);
                    wall.transform.position = new Vector3(wall.transform.position.x, buildingHeight / 2, wall.transform.position.z);

                    // Assign a material
                    wall.GetComponent<MeshRenderer>().material = buildingMaterials[buildingMaterialIndexer];

                    // Parent in hierarchy
                    wall.transform.SetParent(building.transform);
                }

                // Set its transform after rotation
                building.transform.SetParent(minorRoad.transform);
            }
        }
    }

    /// <summary>
    /// Loops through terrain and spawns water/land
    /// </summary>
    void SpawnTerrain()
    {
        // A gameobject in the hierarchy to hold all of the terrain
        GameObject terrainHolder = new GameObject("Terrain");

        // Spawn the grass terrain
        Terrain terrain = Instantiate(prefab_terrain).GetComponent<Terrain>();

        // How big is the cities terrain
        int terrainSize = TerrainSize(cityPtr);

        // Get the max/min values
        float highest = GetTerrainMaximumHeight(cityPtr);
        float lowest = GetTerrainMinimumHeight(cityPtr);

        Debug.Log(string.Format("Highest height: {0}", highest));
        Debug.Log(string.Format("Lowest height: {0}", lowest));

        // Set the terrain this big
        float maxTerrain = highest;
        terrain.terrainData.size = new Vector3(terrainSize, maxTerrain, terrainSize);

        // Get the height data
        float[,] heightData = terrain.terrainData.GetHeights(0, 0, terrainSize, terrainSize);

        //--------------------------------------------
        for (int y = 0; y < terrainSize; y++) { for (int x = 0; x < terrainSize; x++) { heightData[x, y] = 0; } }
        //--------------------------------------------

        // Populate the height data
        for (int y = 0; y < terrainSize; y++)
        {
            for (int x = 0; x < terrainSize; x++)
            {
                // Get the height
                float height = GetTerrainHeightAt(cityPtr, x, y);

                if (height <= 0) { heightData[x, y] = 0; }
                else
                {
                    // Find it's percent in decimal form
                    float decimalPercentOfHeight = height / highest;

                    // Debug.Log(decimalPercentOfHeight);

                    // Assign height
                    heightData[x, y] = decimalPercentOfHeight;
                }
            }
        }

        // Set the height data
        terrain.terrainData.SetHeights(0, 0, heightData);

        // Spawn the water terrain -----------------------------------------------------------------

        // What percent
        float percent = GetWaterHeightPercent(cityPtr);

        // The bottom x percent shal be considered water
        float waterLevel = highest * (percent / 100);

        Debug.Log(string.Format("Water level: {0}", waterLevel));

        for (int x = 0; x < citySize; x++)
        {
            for (int y = 0; y < citySize; y++)
            {
                if (IsWater(cityPtr, x, y))
                {
                    GameObject waterr = Instantiate(prefab_terrainWater, new Vector3(x, waterLevel, y), Quaternion.identity);
                    waterr.transform.SetParent(terrainHolder.transform);
                }
            }
        }
    }
}