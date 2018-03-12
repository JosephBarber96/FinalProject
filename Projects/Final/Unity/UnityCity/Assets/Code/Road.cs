using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Road : MonoBehaviour
{
    [Header("Node")]
    public GameObject node;

    private GameObject start;
    private GameObject end;

    public void SetStart(float x, float y)
    {
        start = Instantiate(node).gameObject;
        start.transform.position = new Vector3(x, transform.position.y, y);
    }

    public void SetEnd(float x, float y)
    {
        end = Instantiate(node).gameObject;
        end.transform.position = new Vector3(x, transform.position.y, y);
    }

    public void Adjust()
    {
        start.transform.LookAt(end.transform);
        end.transform.LookAt(start.transform);

        float distanceBetween = Vector3.Distance(start.transform.position, end.transform.position);

        transform.position = start.transform.position + distanceBetween / 2 * start.transform.forward;
        transform.rotation = start.transform.rotation;
        transform.localScale = new Vector3(transform.localScale.x, transform.localScale.y, distanceBetween / 10);
    }
}
