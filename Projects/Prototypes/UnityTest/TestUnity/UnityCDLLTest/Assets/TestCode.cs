using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System.Text;

public class TestCode : MonoBehaviour
{
    [DllImport("TestDLL.dll")]
    static extern int Add(int a, int b);

    [DllImport("TestDLL.dll")]
    static extern void Sort(int [] ia, int size);

    void Start()
    {
        // Add
        int a = 4, b = 2;
        Debug.Log(string.Format("{0} + {1} = {2}", a, b, Add(a, b)));

        // Sort
        int[] ia = { 10, 7, 8, 4, 6, 7, 2, 1, 5, 7, 2};


            

        Debug.Log("Before sorting: ");
        PrintArray(ia);

        Sort(ia, ia.Length);

        Debug.Log("After sorting:");
        PrintArray(ia);
    }

    void PrintArray(int [] ia)
    {
        StringBuilder nums = new StringBuilder("");
        foreach (var item in ia)
        {
            nums.Append(item.ToString());
            nums.Append(", ");
        }
        nums.Remove(nums.Length-2, 1);
        Debug.Log(nums);
    }
}