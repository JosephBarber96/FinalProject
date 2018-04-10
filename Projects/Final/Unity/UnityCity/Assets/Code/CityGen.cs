using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices; //DLL Import

public class CityGen : MonoBehaviour
{
    private int citySize;

    [Header("Terrain height")]
    [SerializeField]
    [Range(2, 25)]
    private float terrainHeight;

    [Header("Water level (percent)")]
    [SerializeField]
    [Range(0, 50)]
    private float waterLevelInPercent;

    [Header("Building height")]
    [SerializeField]
    [Range(5, 15)]
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
        ref float sx, ref float sz, ref float ex, ref float ez);

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
    [Header("Prefabs (Do not touc")]
    public GameObject prefab_minorRoad;
    public GameObject prefab_majorRoad;
    public GameObject prefab_building;
    public GameObject prefab_terrainGrass;
    public GameObject prefab_terrainWater;
    public GameObject prefab_wall;
    public GameObject prefab_terrain;
    public Material[] buildingMaterials;

    /* Private objects we keep reference to */
    float highestTerrainValue;
    Terrain terrain;

    void Start()
    {
        // Generate the C++ City object
        GenCity();

        // Spawn the terrain
        SpawnTerrain();

        // Spawn roads of the city
        SpawnMajorRoads();
        SpawnMinorRoadsAndBuildings();
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
                float startX = 0, startZ = 0, endX = 0, endZ = 0;
                MajorRoadSegmentPos(cityPtr, i, j, ref startX, ref startZ, ref endX, ref endZ);

                // If this segment is out of bounds, continue without spawning it
                if (ValueOutOfCityBounds(startX) ||
                    ValueOutOfCityBounds(startZ) ||
                    ValueOutOfCityBounds(endX) ||
                    ValueOutOfCityBounds(endZ)) { continue; }

                // Grab the height from the terrain
                float startY = 0, endY = 0;
                startY = terrain.terrainData.GetHeight((int)startX, (int)startZ);
                endY = terrain.terrainData.GetHeight((int)endX, (int)endZ);

                startY += 0.25f;
                endY += 0.25f;

                //if (startX > 0 && startX < citySize && startZ > 0 && startZ < citySize)
                //{
                //    //--------------------------------------------------------------------------------------------------
                //    //--------------------------------------------------------------------------------------------------
                //    // Adjust the terrain to sit flush with the road...
                //    // Find the road vector
                //    Vector2 roadStart = new Vector2(startX, startZ);
                //    Vector2 roadEnd = new Vector2(endX, endZ);
                //    Vector2 roadVector = roadStart - roadEnd;

                //    // Find normalized and magnitude values
                //    Vector2 roadVectorNormalized = roadVector.normalized;
                //    float roadMag = roadVector.magnitude;

                //    // Find height values
                //    float startHeight = startY;
                //    float endHeight = endY;
                //    float heightDifference = startHeight - endHeight;
                //    float heightPerStep = heightDifference / (int)roadMag;

                //    // For the length of the road number of iterations...
                //    for (int k = 0; k < (int)roadMag; k++)
                //    {
                //        // Get the terrain height map
                //        float[,] heights = terrain.terrainData.GetHeights(0, 0, citySize, citySize);

                //        // Get the index of the current position of the road we're on
                //        float indexX = roadStart.x + (roadVectorNormalized.x * k);
                //        float indexZ = roadStart.y + (roadVectorNormalized.y * k);

                //        // Find height - start Height plus heightPetStep multiplied by the current iteration
                //        float heightValue = startHeight + (heightPerStep * k);

                //        // Turn this into a 0-1 value
                //        float decimalPercentOfHeight = heightValue / (endHeight - startHeight);

                //        heights[(int)indexX, (int)indexZ] = decimalPercentOfHeight;
                //    }
                //    //--------------------------------------------------------------------------------------------------
                //    //--------------------------------------------------------------------------------------------------
                //}

                // Instantiate the road
                Road road = Instantiate(prefab_majorRoad, new Vector3(0, 1, 0), Quaternion.identity).GetComponent<Road>();
                road.gameObject.name = string.Format("Major_Road_Segment_{0}", j);

                // Position the road
                road.SetStart(startX, startY, startZ);
                road.SetEnd(endX, endY, endZ);
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
            float roadSX = 0, roadSZ = 0, roadEX = 0, roadEZ = 0;
            MinorRoadPos(cityPtr, i, ref roadSX, ref roadSZ, ref roadEX, ref roadEZ);

            // If this road is out of bounds, continue without spawning it
            if (ValueOutOfCityBounds(roadSX) ||
                ValueOutOfCityBounds(roadSZ) ||
                ValueOutOfCityBounds(roadEX) ||
                ValueOutOfCityBounds(roadEZ)) { continue; }

            // Grab the height from the terrain
            float roadSY = 0, roadEY = 0;
            roadSY = terrain.terrainData.GetHeight((int)roadSX, (int)roadSZ);
            roadEY = terrain.terrainData.GetHeight((int)roadEX, (int)roadEZ);
            roadSY += 0.25f;
            roadEY += 0.25f;

            // Instantiate the road
            Road road = Instantiate(prefab_minorRoad, new Vector3(0, 1, 0), Quaternion.identity).GetComponent<Road>();
            road.gameObject.name = string.Format("Road");

            // Set its position
            road.SetStart(roadSX, roadSY, roadSZ);
            road.SetEnd(roadEX, roadEY, roadEZ);
            road.Adjust();

            // Make it a child in the hierarchy
            road.transform.SetParent(minorRoad.transform);

            /* Buildings... */
            // Find out how many buildings this road has
            int buildingCount = MinorRoadBuildingCount(cityPtr, i);
            for (int j = 0; j < buildingCount; j++)
            {
                // Get the building x, z and y position
                float buildingX = 0, buildingZ = 0;
                MinorRoadBuildingPos(cityPtr, i, j, ref buildingX, ref buildingZ);
                float buildingY = terrain.terrainData.GetHeight((int)buildingX, (int)buildingZ);

                // Get number of lines of the building
                int lineCount = BuildingVertCount(cityPtr, i, j);

                // Create a game object to hold this building
                GameObject building = new GameObject("Building");

                // What material will this building use
                int buildingMaterialIndexer = (int)Random.Range(0, buildingMaterials.Length);

                // Remember the lowest and highest x and y values of the building
                float lowestX = int.MaxValue;
                float lowestZ = int.MaxValue;
                float highestX = int.MinValue;
                float highestZ = int.MinValue;

                // For each edge of the building
                for (int k = 0; k < lineCount; k++)
                {
                    // Get the line
                    float buildingLineSX = 0, buildingLineSZ = 0, buildingLineEX = 0, buildingLineEZ = 0;
                    GetBuildingLine(cityPtr, i, j, k, ref buildingLineSX, ref buildingLineSZ, ref buildingLineEX, ref buildingLineEZ);

                    // See if this line holds any lowest/highest valeus
                    // startX
                    if (buildingLineSX < lowestX) lowestX = buildingLineSX;
                    else if (buildingLineSX > highestX) highestX = buildingLineSX;
                    // startZ
                    if (buildingLineSZ < lowestZ) lowestZ = buildingLineSZ;
                    else if (buildingLineSZ > highestZ) highestZ = buildingLineSZ;
                    // endX
                    if (buildingLineEX < lowestX) lowestX = buildingLineEX;
                    else if (buildingLineEX > highestX) highestX = buildingLineEX;
                    // endZ
                    if (buildingLineEZ < lowestZ) lowestZ = buildingLineEZ;
                    else if (buildingLineEZ > highestZ) highestZ = buildingLineEZ;

                    // Instantiate
                    Wall wall = Instantiate(prefab_wall, new Vector3(0, buildingY, 0), Quaternion.identity).GetComponent<Wall>();
                    wall.SetStart(buildingLineSX, buildingLineSZ);
                    wall.SetEnd(buildingLineEX, buildingLineEZ);
                    wall.Adjust();

                    // Position and extrude the wall (Y axis for both)
                    wall.transform.localScale = new Vector3(wall.transform.localScale.x, buildingHeight, wall.transform.localScale.z);
                    wall.transform.position = new Vector3(wall.transform.position.x, (buildingHeight / 2) + buildingY, wall.transform.position.z);

                    // Assign a material
                    wall.GetComponent<MeshRenderer>().material = buildingMaterials[buildingMaterialIndexer];

                    // Parent in hierarchy
                    wall.transform.SetParent(building.transform);
                }

                // Set its transform after rotation
                building.transform.SetParent(minorRoad.transform);

                // Adjust the terrain to be flush

                // Find width and height of buildings
                float buildingWidth = highestX - lowestX;
                float buildingLength = highestZ - lowestZ;

                // Find the terrain boundaries to adjust
                int terrainStartX = (int)buildingX - (int)buildingWidth; //(int)buildingX - (int)(buildingWidth / 2);
                int terrainEndX = (int)buildingX + (int)buildingWidth; //(int)buildingX + (int)(buildingWidth / 2);

                int terrainStartZ = (int)buildingZ - (int)buildingLength; //(int)buildingZ - (int)(buildingLength / 2);
                int terrainEndZ = (int)buildingZ + (int)buildingLength; //(int)buildingZ + (int)(buildingLength / 2);

                // Adjust the terrainData
                float[,] heights = terrain.terrainData.GetHeights(0, 0, citySize, citySize);
                for (int x = terrainStartX; x < terrainEndX; x++)
                {
                    for (int z = terrainStartZ; z < terrainEndZ; z++)
                    {
                        heights[z, x] = TerrainDecimalValue(buildingY);
                    }
                }
                terrain.terrainData.SetHeights(0, 0, heights);
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
        terrain = Instantiate(prefab_terrain).GetComponent<Terrain>();

        // How big is the cities terrain
        int terrainSize = TerrainSize(cityPtr);

        // Get the max/min values
        highestTerrainValue = GetTerrainMaximumHeight(cityPtr);
        float lowest = GetTerrainMinimumHeight(cityPtr);

        Debug.Log(string.Format("Highest height: {0}", highestTerrainValue));
        Debug.Log(string.Format("Lowest height: {0}", lowest));

        // Set the terrain this big
        terrain.terrainData.size = new Vector3(terrainSize, highestTerrainValue, terrainSize);

        // Get the height data
        float[,] heightData = terrain.terrainData.GetHeights(0, 0, terrainSize, terrainSize);

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
                    heightData[x, y] = TerrainDecimalValue(height);
                }
            }
        }

        // Set the height data
        terrain.terrainData.SetHeights(0, 0, heightData);

        // Spawn the water terrain -----------------------------------------------------------------

        // What percent
        float percent = GetWaterHeightPercent(cityPtr);

        // The bottom x percent shal be considered water
        float waterLevel = highestTerrainValue * (percent / 100);

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

    /// <summary>
    /// Returns the 0-1 value of a given height for use in setting
    /// terrain height
    /// </summary>
    /// <param name="height"></param>
    /// <returns></returns>
    private float TerrainDecimalValue(float height)
    {
        // Find the heights percent in decimal form
        float decimalPercentOfHeight = height / highestTerrainValue;
        return decimalPercentOfHeight;
    }

    /// <summary>
    /// Checks whether a given x or y value is outside of the city terrain bounds
    /// </summary>
    /// <param name="val"></param>
    /// <returns></returns>
    private bool ValueOutOfCityBounds(float val)
    {
        return (val < 0 || val > citySize);
    }

    /// <summary>
    /// Checks whether a given x or y value is outside of the city terrain bounds
    /// </summary>
    /// <param name="val"></param>
    /// <returns></returns>
    private bool ValueOutOfCityBounds(int val)
    {
        return (val < 0 || val > citySize);
    }
}