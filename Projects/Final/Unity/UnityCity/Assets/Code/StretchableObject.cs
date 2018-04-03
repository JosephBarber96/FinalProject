using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class StretchableObject : MonoBehaviour
{
    protected GameObject start;
    protected GameObject end;

    public void SetStart(float x, float z)
    {
        start = new GameObject("StartNode");
        start.transform.position = new Vector3(x, transform.position.y, z);

        start.transform.SetParent(this.gameObject.transform);
    }

    public void SetStart(float x, float y, float z)
    {
        start = new GameObject("StartNode");
        start.transform.position = new Vector3(x, y, z);

        start.transform.SetParent(this.gameObject.transform);
    }

    public void SetEnd(float x, float z)
    {
        end = new GameObject("EndNode");
        end.transform.position = new Vector3(x, transform.position.y, z);

        end.transform.SetParent(this.gameObject.transform);
    }

    public void SetEnd(float x, float y, float z)
    {
        end = new GameObject("EndNode");
        end.transform.position = new Vector3(x, y, z);

        end.transform.SetParent(this.gameObject.transform);
    }

    public abstract void Adjust();
}