//
//  flock.hpp
//  boids
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
    void avoid_others_and_match_velocity(const float& avoiding_factor, const float& min_distance, const float& matching_factor);
    void fill_adj_matrix();
    std::vector<std::vector<float>> m_adj_matrix;
    std::vector<Boid> m_boids;
    int m_max_speed;
    int m_max_vision;
};
