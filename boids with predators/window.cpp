//
//  window.cpp
//  boids with predators
//
//  Created by Jonas Wattendorff on 03.10.21.
//

#include "window.hpp"
#include "flock.hpp"

void run_app()
{
    srand(0);
    
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    
    sf::RenderWindow window(sf::VideoMode(512, 512), "Boids with Predator", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    
    Flock flock(window.getSize(), 50, 5, 7);
    
    while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                    window.close();
                
                if (event.type == sf::Event::Resized)
                {
                    sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                    window.setView(sf::View(visibleArea));
                }
            }
            
            flock.move();
            
            window.clear(sf::Color::White);
            window.draw(flock.predator);
            for (Boid& boid : flock.flock)
            {
                window.draw(boid);
            }
            window.display();
        }
}


