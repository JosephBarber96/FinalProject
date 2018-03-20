using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Wall : StretchableObject
{
    public override void Adjust()
    {
        start.transform.LookAt(end.transform);
        end.transform.LookAt(start.transform);

        float distanceBetween = Vector3.Distance(start.transform.position, end.transform.position);

        transform.position = start.transform.position + distanceBetween / 2 * start.transform.forward;
        transform.rotation = start.transform.rotation;
        transform.localScale = new Vector3(transform.localScale.x, transform.localScale.y, distanceBetween);
    }
}
