//
//  boid.hpp
//  boids with predators
//
//  Created by Jonas Wattendorff on 03.10.21.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Boid : public sf::Drawable, public sf::Transformable
{
public:
    Boid(const sf::Vector2f& pos, const sf::Vector2f& vel, const float scale, const sf::Color color);
    Boid();
    void move_boid();
    sf::Vector2f velocity;
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    const float compute_angle() const;
    sf::VertexArray m_vertices;
};
