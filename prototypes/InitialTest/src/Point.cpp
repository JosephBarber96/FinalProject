#include "Point.h"

std::vector<Point> Point::points;

Point::Point()
{
    //ctor
}


Point::Point(int x, int y)
{
    setPosition(sf::Vector2f(x, y));
    shape = new sf::RectangleShape(sf::Vector2f(1, 1));

    Point.points.push_back(this);
}

Point::~Point()
{
    //dtor
}
