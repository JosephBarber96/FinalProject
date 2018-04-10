using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMovement : MonoBehaviour
{
    [Header("Look vars")]
    public float lookSpeedH = 2f;
    public float lookSpeedV = 2f;
    private float yaw = 0f;
    private float pitch = 0f;

    [Header("Zoom and drag")]
    public float zoomSpeed = 2f;
    public float dragSpeed = 6f;

    [Header("Move speed")]
    public float moveSpeed = 2f;
    private float speedToMove;

    
    void Update()
    {
        InputControl();
    }

    void InputControl()
    {
        // Code source: https://answers.unity.com/questions/666905/in-game-camera-movement-like-editor.html

        // Looking around - RMB
        if (Input.GetMouseButton(1))
        {
            yaw += lookSpeedH * Input.GetAxis("Mouse X");
            pitch -= lookSpeedV * Input.GetAxis("Mouse Y");

            transform.eulerAngles = new Vector3(pitch, yaw, 0f);
        }

        // Camera drag - Middle mouse
        if (Input.GetMouseButton(2))
        {
            transform.Translate(-Input.GetAxisRaw("Mouse X") * Time.deltaTime * dragSpeed, -Input.GetAxisRaw("Mouse Y") * Time.deltaTime * dragSpeed, 0);
        }

        // Sprint?
        if (Input.GetKey(KeyCode.LeftShift))
        {
            speedToMove = moveSpeed *2 ;
        }
        else
        {
            speedToMove = moveSpeed;
        }

        // Movement - Keyboard controlls
        if (Input.GetKey(KeyCode.W))
        {
            transform.position += transform.forward * speedToMove * Time.deltaTime;
        }
        if (Input.GetKey(KeyCode.D))
        {
            transform.position += transform.right * speedToMove * Time.deltaTime;
        }
        if (Input.GetKey(KeyCode.S))
        {
            transform.position -= transform.forward * speedToMove * Time.deltaTime;
        }
        if (Input.GetKey(KeyCode.A))
        {
            transform.position -= transform.right * speedToMove * Time.deltaTime;
        }

        // Quit
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            Application.Quit();
        }
    }
}
