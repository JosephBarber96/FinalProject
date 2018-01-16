#include <iostream>
#include <algorithm>

#include "FloorPlan.h"
#include "V2.h"
#include "Line.h"
#include "BoundingBox.h"
#include "UtilRandom.h"
#include "Utility.h"
#include "Intersection.h"

#include "jbShape.h"
#include "jbTriangle.h"
#include "jbSquare.h"
#include "jbPentagon.h"

FloorPlan::FloorPlan()
{
	shapeCounter[Shape::square] = 0;
	shapeCounter[Shape::rectangle] = 0;
	shapeCounter[Shape::pentagon] = 0;
	shapeCounter[Shape::hexagon] = 0;
}

FloorPlan::~FloorPlan() {}

void FloorPlan::SetBoundingBox(int minx, int miny, int maxx, int maxy)
{
	bb = new BoundingBox(minx, miny, maxx, maxy);
}

void FloorPlan::GenerateShapes()
{
	std::vector<int> limits =
	{
		3, // square
		3, // rectangle
		1, // pentagon
		1, // hexagon
	};

	/*	Old: For creating a random shape.

		// Create a random shape
		index = UtilRandom::Instance()->Random(0, Shape::SHAPE_MAX);
		shape = jbShape::CreateShape(Shape(index));
	*/

	for (int i = 0; i < 3; i++)
	{
		jbShape* shape;
		int index;

		// The first shape is always a rectangle
		if (i == 0 || i == 1)
		{
			index = (int)Shape::square;
			shape = jbShape::CreateShape(Shape::square);
		}
		else if (i == 2)
		{
			index = (int)Shape::hexagon;
			shape = jbShape::CreateShape(Shape::hexagon);
		}
		else
		{
			do
			{
				index = UtilRandom::Instance()->Random(0, Shape::SHAPE_MAX);
				shape = jbShape::CreateShape(Shape(index));
			}
			while (shapeCounter[Shape(index)] >= limits[index]);

		}

		// We now have one more of this shape
		shapeCounter[Shape(index)]++;

		// Place the shape randomly within the bounds of the bounding box
		float shapeX = UtilRandom::Instance()->Random(bb->minX, bb->maxX - shape->getWidth());
		float shapeY = UtilRandom::Instance()->Random(bb->minY, bb->maxY - shape->getHeight());
		shape->SetPosition(shapeX, shapeY);

		// Add the shape to our floor plan
		shapes.push_back(shape);
	}
}

void FloorPlan::GeneratePerimeter()
{
	/*******************************************
		Calculate the points of interseption
	********************************************/

	int intersecCounter = 0;

	// For each shape
	for (auto shape : shapes)
	{
		// Check every other shape
		for (auto otherShape : shapes)
		{

			// Dont check the same shape...
			if (shape->Equal_To(otherShape))
			{
				continue;
			}

			// For each line in original shape...
			for (auto line : shape->lines)
			{
				// Check it against the otherShapes lines
				for (auto otherLine : otherShape->lines)
				{

					V2* intersec = Utility::GetIntersectionPointWithFiniteLines(line->start, line->end, otherLine->start, otherLine->end);

					if (intersec != nullptr)
					{
						line->AddIntersection(intersec, otherLine, otherLine->parent);
						intersecCounter++;
					}
				}
			}
		}
	}

	std::cout << "Intersections: " << intersecCounter << std::endl;

	/*******************************************
		Double-check points of intersection, 
		pass any on that have been missed.
	********************************************/
	for (auto shape : shapes)
	{
		for (auto line : shape->lines)
		{
			for (auto isec : line->intersections)
			{
				// At what point does this intersection occur?
				V2* isecPoint = isec->point;

				// For the intersected line, does this line also contain the same intersection?
				bool foundIntersec = false;
				for (auto otherIntersection : isec->lineIntersected->intersections)
				{
					std::cout << "loop" << std::endl;
					if (*otherIntersection->point == *isecPoint)
					{
						foundIntersec = true;
					}
				}

				// If not, add it. 
				if (!foundIntersec)
				{
					std::cout << "Adding missed intersection point!" << std::endl;
					isec->lineIntersected->AddIntersection(isecPoint, isec->parent, isec->parent->parent);
				}
			}
		}
	}

	/*******************************************
		Remove point of intersection that sit 
		inside of another shape
	********************************************/
	
	// For each shape...
	for (auto shape : shapes)
	{
		// For each line in this shape...
		for (auto line : shape->lines)
		{
			// For each point of intersection this line has...
			for (auto isec : line->intersections)
			{
				// Compare it to every other shape
				for (auto otherShape : shapes)
				{
					// Dont check the same shape
					if (shape->Equal_To(otherShape))
					{
						continue;
					}

					// If this point of intersection is within the otherShape...
					if (otherShape->PointWithin(isec->point))
					{
						// Mark it for deletion
						isec->markedForDeletion = true;
					}
				}
			}
		}
	}

	// For every shape
	for (auto shape : shapes)
	{
		// For every line
		for (auto line : shape->lines)
		{
			// std::cout << "Intersections before: " << line->intersections.size() << std::endl;

			for (std::vector<Intersection*>::iterator it = line->intersections.begin(); it != line->intersections.end(); /**/)
			{
				if ((*it)->markedForDeletion)
				{
					// std::cout << "Removing point of intersection." << std::endl;
					it = line->intersections.erase(it);
				}
				else
				{
					++it;
				}
			}
			// std::cout << "Intersections after: " << line->intersections.size() << std::endl;
			// std::cout << std::endl;
		}
	}

	/***************************************************************************
		Organise the intersections by their distance from the start of the line
	*****************************************************************************/
	
	auto sortByDistanceFromStart = [](Intersection* left, Intersection* right) -> bool
	{
		return (V2::DistanceBetween(*left->parent->start, *left->point) < V2::DistanceBetween(*right->parent->start, *right->point));
	};
	
	for (auto shape : shapes)
	{
		for (auto line : shape->lines)
		{
			std::sort(line->intersections.begin(), line->intersections.end(), sortByDistanceFromStart);
		}
	}


	/*******************************************
			Split the lines up accordingly
	********************************************/

	// For each shape...
	for (auto shape : shapes)
	{
		// For each line...
		for (auto line : shape->lines)
		{
			// Split the line up accordingly. 
			std::vector<Line*> newLines = line->returnLines();

			// Add these returned lines into the floor plans perimeter
			for (auto newLine : newLines)
			{
				perimeterLines.push_back(newLine);
			}
		}
	}

	/*******************************************
	Prune lines that sit inside of another shape
	********************************************/
	
	// For every line...
	for (auto line : perimeterLines)
	{
		// Compare it to every shape...
		for (auto shape : shapes)
		{
			// Dont check the shape this line belongs to
			if (line->parent->Equal_To(shape))
			{
				continue;
			}

			if (shape->PointWithin(line->start))
			{
				line->startWithinShape = true;
			}

			if (shape->PointWithin(line->end))
			{
				line->endWithinShape = true;
			}
		}
	}

	int delCounter = 0;
	for (std::vector<Line*>::iterator it = perimeterLines.begin(); it != perimeterLines.end(); /**/)
	{
		if ((*it)->startWithinShape && (*it)->endWithinShape)
		{
			delCounter++;
			it = perimeterLines.erase(it);
		}
		else
		{
			it++;
		}
	}

	std::cout << "Deleted " << delCounter << " lines." << std::endl;
}

void FloorPlan::DrawPerimeter(sf::RenderWindow* window)
{
	for (auto line : perimeterLines)
	{
		sf::Vertex lineVertices[2] =
		{
			sf::Vertex(sf::Vector2f(line->start->x, line->start->y), sf::Color::Green),
			sf::Vertex(sf::Vector2f(line->end->x, line->end->y), sf::Color::Green)
		};

		window->draw(lineVertices, 2, sf::LineStrip);
	}
}