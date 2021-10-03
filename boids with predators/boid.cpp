//
//  boid.cpp
//  boids with predators
//
//  Created by Jonas Wattendorff on 03.10.21.
//

#include "boid.hpp"

#include <math.h>

Boid::Boid(const sf::Vector2f& pos, const sf::Vector2f& vel, const float scale)
    : velocity(vel), m_vertices(sf::TriangleFan, 4)
{
    m_vertices[0].position = scale * sf::Vector2f(-2.5, 0);
    m_vertices[1].position = scale * sf::Vector2f(-5., -2.5);
    m_vertices[2].position = scale * sf::Vector2f(5., 0);
    m_vertices[3].position = scale * sf::Vector2f(-5., 2.5);
    m_vertices[0].color = sf::Color::Black;
    m_vertices[1].color = sf::Color::Black;
    m_vertices[2].color = sf::Color::Black;
    m_vertices[3].color = sf::Color::Black;
    
    setPosition(pos);
    setRotation(compute_angle());
}

Boid::Boid()
    : velocity(1, 1), m_vertices(sf::TriangleFan, 4)
{
    m_vertices[0].position = sf::Vector2f(-2.5, 0);
    m_vertices[1].position = sf::Vector2f(-5., -2.5);
    m_vertices[2].position = sf::Vector2f(5., 0);
    m_vertices[3].position = sf::Vector2f(-5., 2.5);
    m_vertices[0].color = sf::Color::Black;
    m_vertices[1].color = sf::Color::Black;
    m_vertices[2].color = sf::Color::Black;
    m_vertices[3].color = sf::Color::Black;
    
    setPosition(sf::Vector2f(0, 0));
    setRotation(compute_angle());
}

void Boid::move_boid()
{
    setRotation(compute_angle());
    move(velocity.x, velocity.y);
}

void Boid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_vertices, states);
}

const float Boid::compute_angle() const
{
    float add_angle = 0;
    if (velocity.x < 0 && velocity.y < 0)
            add_angle += 180;
    else if (velocity.x < 0)
        add_angle -= 180;
    
    return atan(velocity.y/velocity.x) * 180./M_PI + add_angle;
}
