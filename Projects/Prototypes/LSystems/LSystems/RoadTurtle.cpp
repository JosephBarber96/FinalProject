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


	int loopNum = 10;
	// Scale it to the length we have to move in small increments
	for (int i = 0; i < loopNum; i++)
	{
		// Scale by (length / LOOPNUM)
		Vec2* scaledMoveVector = *moveVector * (length / loopNum);

		// Find out the new point
		Vec2* newPos = *ourPos + *scaledMoveVector;

		///////////////////////////////////////////////////

		//// Check if this is too close to another node...

		//// For every road..
		//for (auto road : roads)
		//{
		//	// Skip the current road
		//	if (*currentRoad == *road) { continue; }

		//	// For every node in the road
		//	for (auto node : currentRoad->points)
		//	{

		//		if (newPos == node)
		//		{ 
		//			continue;
		//		}

		//		// Skip current node
		//		if (newPos->getX() == node->getX() && newPos->getX() == node->getY())
		//		{ 
		//			continue;
		//		}

		//		std::cout << "newpos (" << newPos->getX() << ", " << newPos->getY() << ") \t node: (" << node->getX() << ", " << node->getY() << std::endl;

		//		float dist = Utility::DistanceBetween(ourPos, node);

		//		dist *= 100;

		//		std::cout << "Dist between: " << dist << std::endl;

		//		if (dist < 0.01f)
		//		{
		//			std::cout << "Road id: " << currentRoad->id << "  Node too close ( " << dist << ")" << std::endl;
		//			std::cout << "newPos = (" << newPos->getX() << ", " << newPos->getY() << ") \t node: (" << node->getX() << ", " << node->getY() << ")" << std::endl;
		//		}
		//	}
		//}


		///////////////////////////////////////////////////

		// Add this new pos to our current road
		currentRoad->AddPoint(newPos);

		// Update ourPos
		ourPos = newPos;

		// This becomes our transform position
		transform->position = newPos;
	}
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