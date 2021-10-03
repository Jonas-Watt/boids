//
//  flock.hpp
//  boids with predators
//
//  Created by Jonas Wattendorff on 03.10.21.
//

#pragma once

#include "boid.hpp"

#include <vector>

class Flock
{
public:
    Flock(const sf::Vector2u& window_size, const int num_flock, const float max_speed_flock, const float max_speed_predator);
    void move();
    std::vector<Boid> flock;
    Boid predator;
private:
};
