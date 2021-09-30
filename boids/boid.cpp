//
//  boid.cpp
//  boids
//
//  Created by Jonas Wattendorff on 30.09.21.
//

#include "boid.hpp"

Boid::Boid()
    : velocity(sf::Vector2f(1, 1)), m_arrow(sf::TriangleFan, 4)
{
    m_arrow[0].position = sf::Vector2f(-5., 0);
    m_arrow[1].position = sf::Vector2f(-10., -5.);
    m_arrow[2].position = sf::Vector2f(10., 0);
    m_arrow[3].position = sf::Vector2f(-10., 5.);
    m_arrow[0].color = sf::Color::Black;
    m_arrow[1].color = sf::Color::Black;
    m_arrow[2].color = sf::Color::Black;
    m_arrow[3].color = sf::Color::Black;

    setPosition(0, 0);
    setRotation(compute_angle());
}

Boid::Boid(float x, float y, float dx, float dy)
    : velocity(sf::Vector2f(dx, dy)), m_arrow(sf::TriangleFan, 4)
{
    m_arrow[0].position = sf::Vector2f(-5., 0);
    m_arrow[1].position = sf::Vector2f(-10., -5.);
    m_arrow[2].position = sf::Vector2f(10., 0);
    m_arrow[3].position = sf::Vector2f(-10., 5.);
    m_arrow[0].color = sf::Color::Black;
    m_arrow[1].color = sf::Color::Black;
    m_arrow[2].color = sf::Color::Black;
    m_arrow[3].color = sf::Color::Black;

    setPosition(x, y);
    setRotation(compute_angle());
}

void Boid::move_boid()
{
    setRotation(compute_angle());
    move(velocity.x, velocity.y);
    if (!(velocity.x > -10) && !(velocity.x < 10))
        std::cout << "boids out!" << std::endl;
}

void Boid::avoid_bounds(const int& width, const int& height, const int& margin, const float& avoiding_factor)
{
    if (getPosition().x <= margin)
        velocity.x += avoiding_factor;
    else if (getPosition().x >= width-margin)
        velocity.x -= avoiding_factor;
    if (getPosition().y <= margin)
        velocity.y += avoiding_factor;
    else if (getPosition().y >= height-margin)
        velocity.y -= avoiding_factor;
}

void Boid::limit_speed(const float& max_speed)
{
    const float speed = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
    if (speed > max_speed)
    {
        velocity.x = velocity.x/speed * max_speed;
        velocity.y = velocity.y/speed * max_speed;
    }
}

void Boid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // our particles don't use a texture
    states.texture = NULL;

    // draw the vertex array
    target.draw(m_arrow, states);
}

float Boid::compute_angle() const
{
    float add_angle = 0;
    if (velocity.x < 0 && velocity.y < 0)
            add_angle += 180;
    else if (velocity.x < 0)
        add_angle -= 180;
    
    return atan(velocity.y/velocity.x) * 180./PI + add_angle;
}
