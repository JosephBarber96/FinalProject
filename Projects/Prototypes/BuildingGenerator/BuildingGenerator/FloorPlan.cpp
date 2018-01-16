#include <iostream>

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

void FloorPlan::GenerateShapes(int num)
{
	std::vector<int> limits =
	{
		2, // square
		2, // rectangle
		1, // pentagon
		1, // hexagon
	};

	

	for (int i = 0; i < num; i++)
	{
		jbShape* shape;
		int index;

		//// The first shape is always a rectangle
		//if (i == 0)
		//{
		//	index = (int)Shape::rectangle;
		//	shape = jbShape::CreateShape(Shape::rectangle);
		//}
		//else
		//{
		//	do
		//	{
		//		// Create a random shape
		//		index = UtilRandom::Instance()->Random(0, Shape::SHAPE_MAX);
		//		shape = jbShape::CreateShape(Shape(index));
		//	}
		//	// While this shape hasn't met its limit
		//	while (shapeCounter[Shape(index)] >= limits[index]);
		//}

		index = (int)Shape::square;
		shape = jbShape::CreateShape(Shape::square);



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
						line->AddIntersection(intersec, otherLine->parent);
						intersecCounter++;
					}
				}
			}
		}
	}

	std::cout << "Intersections: " << intersecCounter << std::endl;

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

			if (shape->PointWithin(line->start) || shape->PointWithin(line->end))
			{
				std::cout << "Marking line for deletion." << std::endl;
				line->markedForDeletion = true;
			}
		}
	}

	int delCounter = 0;
	for (std::vector<Line*>::iterator it = perimeterLines.begin(); it != perimeterLines.end(); /**/)
	{
		if ((*it)->markedForDeletion)
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