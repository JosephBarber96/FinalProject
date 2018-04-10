# Joseph Barber (S175950) - Final Project (IMDCGD399-17YRD)

## Development blog:

https://josephbarberfinalproject.wordpress.com/

## Running the project:

## Standalone:

### Dependances:

**Boost:**

Running the Standalone requires the C++ Boost Library 1.65s for windows.

This can be installed from: https://dl.bintray.com/boostorg/release/1.65.1/binaries/

Download and install boost_1_65_1-msvc-10.0-32.exe to C: 

The include directory is C:\local\boost_1_65_1

**Instructions:**

* Go to Projects\Final\Standalone
* Open ProceduralCityGenerator.sln in Visual Studio 2015
* Set to Release x86
* Run

If an error occurs, abort and re-run.

Follow the instructions printed to the console window to control the application.

**Fixing linker errors:**

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

### Dependances:

**Unity 2017.1 or higher:**

To install Unity, go to: https://store.unity.com/

Install the Free (Personal) version of Unity3D

**Instructions:**

**Running the application:**

* Open the Unity project:
    - Go to Projects\Final\Unity\UnityCity\Assets\Scenes
    - Open Main.unity
* Click Play at the top of the window

The city can take up to 30 seconds to generate. 

**Navigating the city:**

No game-controls are supplies, however the city can be navigated in the Scene window.

Controls:
- W - Move forward
- A - Move left
- S - Move backwards
- D - Move right
- Right-mouse-button - Click, hold and drag to rotate view

**Applying user settings:**

Select the object "ApplicationManager" in the Hierarchy.

From here you can access:

* Terrain height
* Water level in percent
* Building height

Please do not disrupt or change any of the Prefabs settings.
