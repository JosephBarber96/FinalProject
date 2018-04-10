# Joseph Barber (S175950) - Final Project (IMDCGD399-17YRD)

## Development blog:

https://josephbarberfinalproject.wordpress.com/

## Introduction and explanation:

My Final Project was to create a system that could procedurally generate a city. My language of choice was C++. I later, as one of my stretch goals, turned this into a Unity3D library.

This project is split into multiple parts:

* C++ Standalone, using SFML for rendering
* Unity 3D Library

Below are instructions for running both the C++ system and the Unity3D library

## C++Standalone:

### Dependances:

**Boost:**

Running the Standalone requires the C++ Boost Library 1.65s for windows.  
This can be installed from: https://dl.bintray.com/boostorg/release/1.65.1/binaries/  
Download and install boost_1_65_1-msvc-10.0-32.exe to C:\   
The include directory used in the Visual Studio .sln is C:\local\boost_1_65_1  

### Instructions:

* Go to SourceCode\Final\Standalone
* Open ProceduralCityGenerator.sln in Visual Studio 2015
* Set to Release x86
* Build/Run

If an error occurs, abort and re-run.  
Follow the instructions printed to the console window to control the application.

### Fixing linker errors:

Whilst the repository contains the required SFML files, if you get a linker error, follow these steps:  
Add these to the sln properties:

*Additional include directories:*
* $(SolutionDir)\\..\\..\Libraries\SFML-2.4.2\include
* C:\local\boost_1_65_1

*Additional Library Directories:*
* $(SolutionDir)\\..\\..\Libraries\SFML-2.4.2\lib

*Additional Dependancies:*
* sfml-graphics.lib
* sfml-window.lib
* sfml-system.lib

## Unity3D library:

## Running the exe:

* Go to Builds\Unity
* Run ProceduralCity.exe

## Opening and Running the Unity Project:

### Dependances:

**Unity 2017.1 or higher:**

To install Unity, go to: https://store.unity.com/

Install the Free (Personal) version of Unity3D

## Instructions:

**Running the application:**

* Open the Unity project:
    - Go to SourceCode\Final\Unity\UnityCity\Assets\Scenes
    - Open Main.unity
* Click Play at the top of the window

The city can take up to 30 seconds to generate. If you are waiting for over a minute, please close and re-open.

**Applying user settings:**

Select the object "ApplicationManager" in the Hierarchy.

From here you can access:

* Terrain height
* Water level in percent
* Building height

Please do not disrupt or change any of the Prefabs settings.
