//
//  flock.hpp
//  boids v1
//
//  Created by Jonas Wattendorff on 29.09.21.
//

#pragma once

#include <vector>

#include "boid.hpp"

class Flock
{
public:
    Flock(const int& n, const int& max_speed, const int& max_vision);
    void move(const sf::Vector2u& window_size);
    void draw(sf::RenderWindow& window) const;
private:
    void avoid_others(const float& avoiding_factor, const float& min_distance);
    void match_velocity(const float& matching_factor);
    void fly_to_center(const float& centering_factor);
    std::vector<Boid> m_boids;
    const int m_max_speed;
    const int m_max_vision;
};
