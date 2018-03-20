using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices; //DLL Import

public class CityGen : MonoBehaviour
{
    /* An IntPtr for holding the memory address of the C++ City object */
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
    static extern bool IsWater(System.IntPtr city, int x, int y);

    /* Prefabs */
    [Header("Prefabs")]
    public GameObject prefab_minorRoad;
    public GameObject prefab_majorRoad;
    public GameObject prefab_building;
    public GameObject prefab_terrainGrass;
    public GameObject prefab_terrainWater;
    public GameObject prefab_wall;
    public Material[] buildingMaterials;

    [Header("Variables")]
    public float buildingHeight;

    private float grassY = -0.2f;
    private float waterY = -0.1f;
    private float minorRoadY = 0f;
    private float majorRoadY = 0.1f;

    /* Data */
    int citySize = 512;

    void Start()
    {
        // Generate the C++ City object
        GenCity();

        // Spawn roads of the city
        SpawnMajorRoads();
        SpawnMinorRoadsAndBuildings();

        // Spawn the terrain
        SpawnTerrain();
    }

    /// <summary>
    /// Generates the C++ City object, holds 
    /// a pointer to it in memory
    /// </summary>
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
                Road road = Instantiate(prefab_majorRoad, new Vector3(0, majorRoadY, 0), Quaternion.identity).GetComponent<Road>();
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
            Road road = Instantiate(prefab_minorRoad, new Vector3(0, minorRoadY, 0), Quaternion.identity).GetComponent<Road>();
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
                    Wall wall = Instantiate(prefab_wall, new Vector3(0, minorRoadY, 0), Quaternion.identity).GetComponent<Wall>();
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
        GameObject terrain = new GameObject("Terrain");
        
        // Spawn the grass terrain
        float scaleAmount = (citySize / 2) / 10;
        GameObject grass = Instantiate(prefab_terrainGrass, new Vector3(citySize / 2, grassY, citySize / 2), Quaternion.identity);
        grass.transform.localScale = new Vector3(citySize / 10, 1, citySize / 10);
        grass.transform.SetParent(terrain.transform);

        // Spawn the water terrain
        for (int x = 0; x < citySize; x++)
        {
            for (int y = 0; y < citySize; y++)
            {
                if (IsWater(cityPtr, x, y))
                {
                    GameObject water = Instantiate(prefab_terrainWater, new Vector3(x, waterY, y), Quaternion.identity);
                    water.transform.SetParent(terrain.transform);
                }
                    
            }
        }
    }
}