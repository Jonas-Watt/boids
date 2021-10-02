//
//  boid.hpp
//  boids
//
//  Created by Jonas Wattendorff on 30.09.21.
//

#pragma once

#include <iostream>
#include <vector>
#include <math.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#define PI 3.14159265

class Boid : public sf::Drawable, public sf::Transformable
{
public:
    Boid();
    Boid(float x, float y, float dx, float dy);
    void move_boid();
    void avoid_bounds(const int& width, const int& height, const int& margin, const float& avoiding_factor);
    void limit_speed(const float& max_speed);
    sf::Vector2f velocity;
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    float compute_angle() const;
    sf::VertexArray m_arrow;
};
