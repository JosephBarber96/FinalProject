#ifndef POINT_H
#define POINT_H

#include <SFML/Graphics.hpp>
#include <vector>


class Point : sf::Transformable
{
    public:

        static std::vector<Point> points;
        Point();
        Point(int x, int y);
        virtual ~Point();

        sf::RectangleShape* getShape() { return shape; }

    protected:

        sf::RectangleShape* shape;

    private:
};

#endif // POINT_H
