using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CanvasController : MonoBehaviour
{
    [Header("Text")]
    public Text loadingText;

    public void HideLoadingText()
    {
        loadingText.text = "";
    }
}