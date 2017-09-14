#include <SFML/Graphics.hpp>
#include <stdlib.h>

#include "Point.h"

int min = 0;
int max = 500;

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(max, max), "SFML window");

    // Create points
    for (int i = 0; i < 15; i++)
    {
        srand(time(NULL));

        int x = rand() % max;
        int y = rand() % max;

        Point point = Point(x, y);
    }


	// Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear screen
        window.clear();

        // Draw to the screen
        for (auto i : Point.points)
        {
            window.draw(Point.points[i].getShape(), Point.points[i].getTransform());
        }

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
