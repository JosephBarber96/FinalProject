#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>

#include "RoadTurtle.h"
#include "Road.h"
#include "Vec2.h"
#include "Transform.h"
#include "Utility.h"
#include "PopulationPoint.h"


RoadTurtle::RoadTurtle() {}

RoadTurtle::~RoadTurtle() {}

void RoadTurtle::SetPos(Vec2* newPos)
{
	transform->position = newPos;
}

void RoadTurtle::SetPos(PopulationPoint* newPos)
{
	transform->position = new Vec2(newPos->getX(), newPos->getY());
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

		//		//std::cout << "newpos (" << newPos->getX() << ", " << newPos->getY() << ") \t node: (" << node->getX() << ", " << node->getY() << std::endl;

		//		float dist = Utility::DistanceBetween(newPos, node);

		//		dist *= 100;

		//		//std::cout << "Dist between: " << dist << std::endl;

		//		if (dist < 0.1f)
		//		{
		//			std::cout << "Road id: " << currentRoad->id << "  Node too close (dist = " << dist << ")" << std::endl;
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

bool RoadTurtle::FindNewTarget(std::vector<PopulationPoint*>& popPoints)
{
	auto lowestDist = FLT_MAX;
	PopulationPoint* closest = nullptr;

	for (auto point : popPoints)
	{
		if (point->getMarked()) { continue; }

		float thisDist = Utility::DistanceBetween(point, transform->position);

		if (thisDist < lowestDist)
		{
			lowestDist = thisDist;
			closest = point;
		}
	}

	if (closest == nullptr) return false;

	SetCurrentTarget(closest);
	return true;
}

void RoadTurtle::SetCurrentTarget(PopulationPoint* newTarget)
{
	currentTarget = newTarget;

	float x1 = transform->position->getX();
	float y1 = transform->position->getY();
	float x2 = currentTarget->getX();
	float y2 = currentTarget->getY();

	//auto dot = x1*x2 + y1*y2;
	//auto det = x1*y2 - y1*x2;
	//auto angle = atan2f(det, dot);

	float angle = atan2(y1 - y2, x1 - x2);
	angle = angle * 180 / M_PI;

	transform->rotation = angle;
	transform->rotation += 180;

	std::cout << "New target." << 
		"Turtle pos (" << transform->position->getX() << ", " << transform->position->getY() << ") " 
		<< "Target pos (" << currentTarget->getX() << ", " << currentTarget->getY() << ") "
		<< "Rotation: " << transform->rotation << std::endl;
}

void RoadTurtle::AddRoad(Road* newRoad)
{
	roads.push_back(newRoad);
}

void RoadTurtle::AddRoad(Vec2* roadStart, Vec2* roadEnd)
{
	roads.push_back(new Road(roadStart));
}