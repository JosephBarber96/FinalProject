#include <iostream>

#include "RoadTurtle.h"
#include "Road.h"
#include "Vec2.h"
#include "Transform.h"
#include "Utility.h"


RoadTurtle::RoadTurtle() {}

RoadTurtle::~RoadTurtle() {}

void RoadTurtle::SetPos(Vec2* newPos)
{
	transform->position = newPos;
}

void RoadTurtle::SetAngle(float deg)
{
	transform->rotation = deg;
}

void RoadTurtle::SetStartingTransform(Vec2* startPos, float startAngle)
{
	transform = new Transform();
	transform->position = startPos;

	transform->rotation = startAngle;

	currentRoad = new Road(transform->position);
	AddRoad(currentRoad);
}

void RoadTurtle::ExtendRoad(float length)
{
	// Get the current position (the most recent point on our current road)
	Vec2* ourPos = currentRoad->points[currentRoad->points.size()-1];

	// Check that this road hasn't been pruned
	if (currentRoad->roadPruned) { return; }

	// Get the vector of our angle
	Vec2* moveVector = Vec2::AngleToVector(transform->rotation);

	// Scale it for the length we have to move
	Vec2* scaledMoveVector = *moveVector * length;

	// Find out our ending position
	Vec2* newPos = *ourPos + *scaledMoveVector;

	// Add this new pos to our current road
	currentRoad->AddPoint(newPos);

	transform->position = newPos;
}

void RoadTurtle::Rotate(float deg)
{
	transform->rotation += deg;
}

void RoadTurtle::Branch()
{
	currentRoad = new Road(transform->position);
	
	AddRoad(currentRoad);
}

void RoadTurtle::Push()
{
	roadStack.push(currentRoad);
	transformStack.push(transform);

	currentRoad = new Road(transform->position);
	AddRoad(currentRoad);

	transform = new Transform(transformStack.top()->rotation, transformStack.top()->position);
}

void RoadTurtle::Pop()
{
	currentRoad = roadStack.top();
	roadStack.pop();

	transform = transformStack.top();
	transformStack.pop();
}

void RoadTurtle::AddRoad(Road* newRoad)
{
	roads.push_back(newRoad);
}

void RoadTurtle::AddRoad(Vec2* roadStart, Vec2* roadEnd)
{
	roads.push_back(new Road(roadStart));
}