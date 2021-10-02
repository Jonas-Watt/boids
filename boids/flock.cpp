//
//  flock.cpp
//  boids
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
    avoid_others(1., 20);
    match_velocity(1.);
    for (Boid& boid : m_boids)
    {
        boid.limit_speed(m_max_speed);
        boid.avoid_bounds(window_size.x, window_size.y, 100, 0.3);
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
    std::vector<sf::Vector2f> av_vel_neighbours(m_boids.size());
    std::vector<int> av_num_neighbours(m_boids.size());
    for (int i = 0; i < m_boids.size(); i++)
    {
        for (int j = i+1; j < m_boids.size(); j++)
        {
            float distance_sqr = (m_boids[i].getPosition().x-m_boids[j].getPosition().x) * (m_boids[i].getPosition().x-m_boids[j].getPosition().x)
                                 + (m_boids[i].getPosition().y-m_boids[j].getPosition().y) * (m_boids[i].getPosition().y-m_boids[j].getPosition().y);
            if (distance_sqr <= min_distance * min_distance)
            {
                const sf::Vector2f tmp = (m_boids[j].getPosition() - m_boids[i].getPosition())/min_distance;
                av_vel_neighbours[i] -= tmp;
                av_vel_neighbours[j] += tmp;
                av_num_neighbours[i]++;
                av_num_neighbours[j]++;
            }
        }
    }
    for (int i = 0; i < m_boids.size(); i++)
    {
        if (av_num_neighbours[i] > 0)
            m_boids[i].velocity += av_vel_neighbours[i]/(float)av_num_neighbours[i] * avoiding_factor;
    }
}

void Flock::match_velocity(const float& matching_factor)
{
    std::vector<sf::Vector2f> ma_vel_neighbours(m_boids.size());
    std::vector<int> ma_num_neighbours(m_boids.size());
    for (int i = 0; i < m_boids.size(); i++)
    {
        for (int j = i+1; j < m_boids.size(); j++)
        {
            float distance_sqr = (m_boids[i].getPosition().x-m_boids[j].getPosition().x) * (m_boids[i].getPosition().x-m_boids[j].getPosition().x)
                                 + (m_boids[i].getPosition().y-m_boids[j].getPosition().y) * (m_boids[i].getPosition().y-m_boids[j].getPosition().y);
            if (distance_sqr <= m_max_vision * m_max_vision)
            {
                ma_vel_neighbours[i] += sf::Vector2f(m_boids[j].velocity.x/(float)m_max_speed * abs(m_boids[j].getPosition().x - m_boids[i].getPosition().x)/m_max_vision, m_boids[j].velocity.y/(float)m_max_speed * abs(m_boids[j].getPosition().y - m_boids[i].getPosition().y)/m_max_vision);;
                ma_vel_neighbours[j] += sf::Vector2f(m_boids[i].velocity.x/(float)m_max_speed * abs(m_boids[i].getPosition().x - m_boids[j].getPosition().x)/m_max_vision, m_boids[i].velocity.y/(float)m_max_speed * abs(m_boids[i].getPosition().y - m_boids[j].getPosition().y)/m_max_vision);;
                ma_num_neighbours[i]++;
                ma_num_neighbours[j]++;
            }
        }
    }
    for (int i = 0; i < m_boids.size(); i++)
    {
        if (ma_num_neighbours[i] > 0)
            m_boids[i].velocity += ma_vel_neighbours[i]/(float)ma_num_neighbours[i] * matching_factor;
    }
}
