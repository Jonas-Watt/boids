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
    Flock(const sf::Vector2u& window_size, const int num_flock, const float max_speed_flock, const float max_speed_predator, const float max_vision_flock, const float max_vision_predator);
    void move(const sf::Vector2u& window_size);
    std::vector<Boid> flock;
    Boid predator;
private:
    void avoid_predator(const float avoiding_factor);
    void avoid_others(const float avoiding_factor, const float min_distance);
    void match_velocity(const float matching_factor);
    void fly_to_center(const float centering_factor);
    void avoid_bounds(const sf::Vector2u& window_size, const int margin, const float avoiding_factor);
    void limit_speed(const float max_speed_flock, const float max_speed_predator, const float limiting_factor);
    void hunt_flock(const float hunting_factor);
    const int m_num_flock;
    const float m_max_vision_flock;
    const float m_max_vision_predator;
    const float m_max_speed_flock;
    const float m_max_speed_predator;
};
