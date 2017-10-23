#include "Turtle.h"
#include "Vec2.h"
#include "Line.h"

Turtle::Turtle()
	:
	angle(0),
	position(new Vec2(0, 0))
{

}

Turtle::~Turtle() {}

// Public

void Turtle::FaceAngle(float deg)
{
	// Set our angle
	angle = deg;
}

void Turtle::Rotate(float deg)
{
	// Rotate
	angle += deg;
}

void Turtle::Move(float length)
{
	// Remember our startPos
	Vec2* startPos = position;

	// Get the vector of our angle
	Vec2* moveVector = Vec2::AngleToVector(angle);

	// Scale it for the length we have to move
	Vec2* scaledMoveVector = *moveVector * length;

	// Find out our ending position
	Vec2* newPos = *startPos + *scaledMoveVector;
	
	// Set our new position
	position = newPos;
}

void Turtle::DrawLine(float length)
{
	// Get our starting position
	Vec2* startPos = position;

	// Move
	Move(length);

	// Get our new position
	Vec2* endPos = position;

	// Create a line
	AddLine(startPos, endPos);
}

void Turtle::PushPosition()
{
	// Push our position onto the stack
	posStack.push(position);
}

void Turtle::PopPosition()
{
	// Move to the position at the top of the stack
	position = posStack.top();

	// Pop this off of the stack
	posStack.pop();
}

// Private

void Turtle::AddLine(Vec2* start, Vec2* end)
{
	Line* line = new Line(start, end);
	lines.push_back(line);
}