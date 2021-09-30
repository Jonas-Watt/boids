//
//  main.cpp
//  boids
//
//  Created by Jonas Wattendorff on 28.09.21.
//

#include <iostream>
#include <vector>
#include <math.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "flock.hpp"
#include "boid.hpp"

// Keine Pointer, sondern ganze Boids

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    
    sf::RenderWindow window;
    window.create(sf::VideoMode(750, 750), "Boids", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    
    Flock flock(15, 5, 100);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }
        
        flock.move(window.getSize());
        
        window.clear(sf::Color::White);
        flock.draw(window);
        window.display();
    }
    return 0;
}
