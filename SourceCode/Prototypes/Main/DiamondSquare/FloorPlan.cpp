#include "FloorPlan.h"
#include "BoundingBox.h"
#include "jbShape.h"
#include "shapeLine.h"
#include "UtilRandom.h"
#include "Intersection.h"
#include "V2.h"
#include "Utility.h"

FloorPlan::FloorPlan() {}

FloorPlan::~FloorPlan() {}

void FloorPlan::SetBoundingBox(int xOri, int yOri, int wid, int hei)
{
	bb = new BoundingBox();
	bb = new BoundingBox(xOri, yOri, wid, hei);
}

void FloorPlan::GenerateShapes(int num)
{
	float width, height;
	width = bb->width;

	float shapeSize = width * 0.51f;

	for (int i = 0; i < num; i++)
	{
		jbShape* shape = jbShape::CreateShape(shapeSize);

		float shapeX, shapeY;
		shapeX = UtilRandom::Instance()->RandomFloat(bb->xOrigin, (bb->xOrigin + bb->width) - shapeSize);
		shapeY = UtilRandom::Instance()->RandomFloat(bb->yOrigin, (bb->yOrigin + bb->height) - shapeSize);

		shape->SetPosition(shapeX, shapeY);

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
						isec->MarkForDeletion();
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
				if ((*it)->IsMarkedForDeletion())
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
			std::vector<shapeLine*> newLines = line->returnLines();

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
				line->MarkForDeletion();
			}
		}
	}

	int delCounter = 0;
	for (std::vector<shapeLine*>::iterator it = perimeterLines.begin(); it != perimeterLines.end(); /**/)
	{
		if ((*it)->IsMarkedForDeletion())
		{
			delCounter++;
			it = perimeterLines.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void FloorPlan::DrawPerimeter(sf::RenderWindow *window)
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