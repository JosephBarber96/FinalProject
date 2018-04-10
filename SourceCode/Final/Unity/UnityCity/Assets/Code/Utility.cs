using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class Utility
{
    public static float VectorToDegree(float x, float y)
    {
        return (float)Mathf.Atan2(y, x) * 180 / Mathf.PI;
    }

    public static float VectorToDegree(Vector2 angle)
    {
        return (float)Mathf.Atan2(angle.y, angle.x) * 180 / Mathf.PI;
    }

    public static GameObject CreateMesh(float width, float height)
    {
        GameObject go = new GameObject("Mesh");

        // Mesh renderer and filter
        MeshFilter mf = go.AddComponent(typeof(MeshFilter)) as MeshFilter;
        MeshRenderer mr = go.AddComponent(typeof(MeshRenderer)) as MeshRenderer;

        Mesh m = new Mesh();

        // Verts 
        m.vertices = new Vector3[]
        {
            new Vector3(0, 0, 0),
            new Vector3(width, 0, 0),
            new Vector3(width, height, 0),
            new Vector3(0, height, 0)
        };

        // UV for texture
        m.uv = new Vector2[]
        {
            new Vector2(0, 0),
            new Vector2(0, 1),
            new Vector2(1, 1),
            new Vector2(1, 0)
        };

        // Tri's to render mesh
        m.triangles = new int[]
        {
            0, 1, 2,
            0, 2, 3
        };

        // Assign mesh
        mf.mesh = m;

        // Recalculate normals and bounds
        m.RecalculateBounds();
        m.RecalculateNormals();

        return go;
    }

}
