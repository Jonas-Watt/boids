//
//  main.cpp
//  boids
//
//  Created by Jonas Wattendorff on 28.09.21.
//

#include <iostream>
#include <vector>
#include <math.h>
#include <filesystem>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "flock.hpp"
#include "boid.hpp"

// Keine Pointer, sondern ganze Boids

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 5;
    
    sf::RenderWindow window;
    window.create(sf::VideoMode(750, 750), "Boids", sf::Style::Default, settings);
    window.setFramerateLimit(30);
    
    Flock flock(1000, 5, 100);
    
    int fps = 0;
    sf::Clock clock;
    sf::Time previousTime = clock.getElapsedTime();
    sf::Time currentTime;
    
    sf::Font font;
    std::filesystem::current_path("/Users/jonas/Dev/boids/boids");
    font.loadFromFile("Andale Mono.ttf");
    
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(12);
    text.setFillColor(sf::Color::Black);
    
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
        
        currentTime = clock.getElapsedTime();
        
        if (currentTime.asSeconds() - previousTime.asSeconds() >= 1)
        {
            text.setString(std::to_string(fps));
            previousTime = currentTime;
            fps = 0;
        }
        fps++;
        
        flock.move(window.getSize());
        
        window.clear(sf::Color::White);
        flock.draw(window);
        window.draw(text);
        window.display();
    }
    return 0;
}
