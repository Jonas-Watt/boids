//
//  flock.cpp
//  boids with predators
//
//  Created by Jonas Wattendorff on 03.10.21.
//

#include "flock.hpp"

Flock::Flock(const sf::Vector2u& window_size, const int num_flock, const float max_speed_flock, const float max_speed_predator)
{
    for (int i = 0; i < num_flock; i++)
    {
        flock.push_back(Boid(sf::Vector2f(rand() % window_size.x, rand() % window_size.y), sf::Vector2f(rand() % (int)max_speed_predator, rand() % (int)max_speed_predator), 1));
    }
    predator = Boid(sf::Vector2f(rand() % window_size.x, rand() % window_size.y), sf::Vector2f(rand() % (int)max_speed_predator, rand() % (int)max_speed_predator), 2);
}

void Flock::move()
{
    predator.move_boid();
    for (Boid& boid : flock)
    {
        boid.move_boid();
    }
}
