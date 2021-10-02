//
//  flock.cpp
//  boids v1
//
//  Created by Jonas Wattendorff on 29.09.21.
//

#include "flock.hpp"

Flock::Flock(const int& n, const int& max_speed, const int& max_vision)
    : m_max_speed(max_speed), m_max_vision(max_vision)
{
    srand(0);
    for (int i = 0; i < n; i++)
    {
        m_boids.push_back(Boid(rand() % 750, rand() % 750, rand() % 2*max_speed - max_speed, rand() % 2*max_speed - max_speed));
    }
}

void Flock::move(const sf::Vector2u& window_size)
{
    match_velocity(0.3);
    avoid_others(0.7, 10);
    fly_to_center(0.3);
    for (Boid& boid : m_boids)
    {
        boid.limit_speed(m_max_speed);
        boid.avoid_bounds(window_size.x, window_size.y, 200, 0.1);
        boid.move_boid();
    }
}

void Flock::draw(sf::RenderWindow& window) const
{
    for (const Boid& boid : m_boids)
    {
        window.draw(boid);
    }
//    for (int i = 0; i < m_boids.size(); i++)
//    {
//        for (int j = i+1; j < m_boids.size(); j++)
//        {
//            if (pow(m_boids[i].getPosition().x-m_boids[j].getPosition().x, 2)
//                + pow(m_boids[i].getPosition().y-m_boids[j].getPosition().y, 2) <= pow(m_max_vision, 2))
//            {
//                sf::VertexArray line(sf::Lines, 2);
//                line[0].position = m_boids[i].getPosition();
//                line[1].position = m_boids[j].getPosition();
//                line[0].color = sf::Color::Black;
//                line[1].color = sf::Color::Black;
//                window.draw(line);
//            }
//        }
//    }
}

//TODO: Liste der Positionen in Flock behalten

void Flock::avoid_others(const float& avoiding_factor, const float& min_distance)
{
    for (int i = 0; i < m_boids.size(); i++)
    {
        for (int j = i+1; j < m_boids.size(); j++)
        {
            float distance_sqr = (m_boids[i].getPosition().x-m_boids[j].getPosition().x) * (m_boids[i].getPosition().x-m_boids[j].getPosition().x) + (m_boids[i].getPosition().y-m_boids[j].getPosition().y) * (m_boids[i].getPosition().y-m_boids[j].getPosition().y);
            if (distance_sqr <= min_distance * min_distance)
            {
                const sf::Vector2f tmp = (m_boids[j].getPosition() - m_boids[i].getPosition())/min_distance * avoiding_factor;
                m_boids[i].velocity -= tmp;
                m_boids[j].velocity += tmp;
            }
        }
    }
}

void Flock::match_velocity(const float& matching_factor)
{
    std::vector<sf::Vector2f> vel_neighbours(m_boids.size());
    std::vector<int> num_neighbours(m_boids.size());
    for (int i = 0; i < m_boids.size(); i++)
    {
        for (int j = i+1; j < m_boids.size(); j++)
        {
            float distance_sqr = (m_boids[i].getPosition().x-m_boids[j].getPosition().x) * (m_boids[i].getPosition().x-m_boids[j].getPosition().x) + (m_boids[i].getPosition().y-m_boids[j].getPosition().y) * (m_boids[i].getPosition().y-m_boids[j].getPosition().y);
            if (distance_sqr <= m_max_vision * m_max_vision)
            {
                vel_neighbours[i] += sf::Vector2f(m_boids[j].velocity.x/(float)m_max_speed * abs(m_boids[j].getPosition().x - m_boids[i].getPosition().x)/m_max_vision, m_boids[j].velocity.y/(float)m_max_speed * abs(m_boids[j].getPosition().y - m_boids[i].getPosition().y)/m_max_vision);
                vel_neighbours[j] += sf::Vector2f(m_boids[i].velocity.x/(float)m_max_speed * abs(m_boids[i].getPosition().x - m_boids[j].getPosition().x)/m_max_vision, m_boids[i].velocity.y/(float)m_max_speed * abs(m_boids[i].getPosition().y - m_boids[j].getPosition().y)/m_max_vision);
                num_neighbours[i]++;
                num_neighbours[j]++;
            }
        }
    }
    for (int i = 0; i < m_boids.size(); i++)
    {
        if (num_neighbours[i] > 0)
            m_boids[i].velocity += vel_neighbours[i]/(float)num_neighbours[i] * matching_factor;
    }
}

void Flock::fly_to_center(const float& centering_factor)
{
    std::vector<sf::Vector2f> center_neighbours(m_boids.size());
    std::vector<int> num_neighbours(m_boids.size());
    for (int i = 0; i < m_boids.size(); i++)
    {
        for (int j = i+1; j < m_boids.size(); j++)
        {
            float distance_sqr = (m_boids[i].getPosition().x-m_boids[j].getPosition().x) * (m_boids[i].getPosition().x-m_boids[j].getPosition().x) + (m_boids[i].getPosition().y-m_boids[j].getPosition().y) * (m_boids[i].getPosition().y-m_boids[j].getPosition().y);
            if (distance_sqr <= m_max_vision * m_max_vision)
            {
                const sf::Vector2f tmp = (m_boids[j].getPosition() - m_boids[i].getPosition())/(float)m_max_vision;
                center_neighbours[i] += tmp;
                center_neighbours[j] -= tmp;
                num_neighbours[i]++;
                num_neighbours[j]++;
            }
        }
    }
    for (int i = 0; i < m_boids.size(); i++)
    {
        if (num_neighbours[i] > 0)
            m_boids[i].velocity += center_neighbours[i]/(float)num_neighbours[i] * centering_factor;
    }
}
