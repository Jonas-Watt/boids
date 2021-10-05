//
//  flock.cpp
//  boids with predators
//
//  Created by Jonas Wattendorff on 03.10.21.
//

#include "flock.hpp"

#include <math.h>

Flock::Flock(const sf::Vector2u& window_size, const int num_flock, const float max_speed_flock, const float max_speed_predator, const float max_vision_flock, const float max_vision_predator)
    : m_max_vision_flock(max_vision_flock), m_max_vision_predator(max_vision_predator), m_num_flock(num_flock), m_max_speed_flock(max_speed_flock), m_max_speed_predator(max_speed_predator)
{
    for (int i = 0; i < num_flock; i++)
    {
        flock.push_back(Boid(sf::Vector2f(rand() % window_size.x, rand() % window_size.y), sf::Vector2f(rand() % (int)max_speed_predator, rand() % (int)max_speed_predator), 1, sf::Color::Black));
    }
    predator = Boid(sf::Vector2f(rand() % window_size.x, rand() % window_size.y), sf::Vector2f(rand() % (int)max_speed_predator, rand() % (int)max_speed_predator), 2, sf::Color::Blue);
}

void Flock::move(const sf::Vector2u& window_size)
{
    avoid_predator(0.5);
    avoid_others(0.7, 10);
    match_velocity(0.3);
    fly_to_center(0.3);
    avoid_bounds(window_size, 100, 0.1);
    limit_speed(m_max_speed_flock, m_max_speed_predator, 0.3);
    
    hunt_flock(0.7);
    
    predator.move_boid();
    for (Boid& boid : flock)
    {
        boid.move_boid();
    }
}

void Flock::avoid_bounds(const sf::Vector2u& window_size, const int margin, const float avoiding_factor)
{
    if (predator.getPosition().x <= margin)
        predator.velocity.x += avoiding_factor;
    else if (predator.getPosition().x >= window_size.x-margin)
        predator.velocity.x -= avoiding_factor;
    if (predator.getPosition().y <= margin)
        predator.velocity.y += avoiding_factor;
    else if (predator.getPosition().y >= window_size.y-margin)
        predator.velocity.y -= avoiding_factor;
    for (Boid& boid : flock)
    {
        if (boid.getPosition().x <= margin)
            boid.velocity.x += avoiding_factor;
        else if (boid.getPosition().x >= window_size.x-margin)
            boid.velocity.x -= avoiding_factor;
        if (boid.getPosition().y <= margin)
            boid.velocity.y += avoiding_factor;
        else if (boid.getPosition().y >= window_size.y-margin)
            boid.velocity.y -= avoiding_factor;
    }
}

void Flock::limit_speed(const float max_speed_flock, const float max_speed_predator, const float limiting_factor)
{
    const float speed_sqr = predator.velocity.x * predator.velocity.x + predator.velocity.y * predator.velocity.y;
    if (speed_sqr > max_speed_predator * max_speed_predator)
    {
        predator.velocity -= predator.velocity * limiting_factor;
    }
    for (Boid& boid : flock)
    {
        const float speed_sqr = boid.velocity.x * boid.velocity.x + boid.velocity.y * boid.velocity.y;
        if (speed_sqr > max_speed_flock * max_speed_flock)
        {
            boid.velocity -= boid.velocity * limiting_factor;
        }
    }
}

void Flock::fly_to_center(const float centering_factor)
{
    std::vector<sf::Vector2f> center_neighbours(flock.size());
    std::vector<int> num_neighbours(flock.size());
    for (int i = 0; i < flock.size(); i++)
    {
        for (int j = i+1; j < flock.size(); j++)
        {
            float distance_sqr = (flock[i].getPosition().x-flock[j].getPosition().x) * (flock[i].getPosition().x-flock[j].getPosition().x) + (flock[i].getPosition().y-flock[j].getPosition().y) * (flock[i].getPosition().y-flock[j].getPosition().y);
            if (distance_sqr <= m_max_vision_flock * m_max_vision_flock)
            {
                const sf::Vector2f tmp = (flock[j].getPosition() - flock[i].getPosition())/(float)m_max_vision_flock;
                center_neighbours[i] += tmp;
                center_neighbours[j] -= tmp;
                num_neighbours[i]++;
                num_neighbours[j]++;
            }
        }
    }
    for (int i = 0; i < flock.size(); i++)
    {
        if (num_neighbours[i] > 0)
            flock[i].velocity += center_neighbours[i]/(float)num_neighbours[i] * centering_factor;
    }
}

void Flock::match_velocity(const float matching_factor)
{
    std::vector<sf::Vector2f> vel_neighbours(flock.size());
    std::vector<int> num_neighbours(flock.size());
    for (int i = 0; i < flock.size(); i++)
    {
        for (int j = i+1; j < flock.size(); j++)
        {
            float distance_sqr = (flock[i].getPosition().x-flock[j].getPosition().x) * (flock[i].getPosition().x-flock[j].getPosition().x) + (flock[i].getPosition().y-flock[j].getPosition().y) * (flock[i].getPosition().y-flock[j].getPosition().y);
            if (distance_sqr <= m_max_vision_flock * m_max_vision_flock)
            {
                vel_neighbours[i] += sf::Vector2f(flock[j].velocity.x/(float)m_max_speed_flock * abs(flock[j].getPosition().x - flock[i].getPosition().x)/m_max_vision_flock, flock[j].velocity.y/(float)m_max_speed_flock * abs(flock[j].getPosition().y - flock[i].getPosition().y)/m_max_vision_flock);
                
                
                //Duplicated from distance_sqr maybe x_distance_sqr and y_distance_sqr??
                
                
                vel_neighbours[j] += sf::Vector2f(flock[i].velocity.x/(float)m_max_speed_flock * abs(flock[i].getPosition().x - flock[j].getPosition().x)/m_max_vision_flock, flock[i].velocity.y/(float)m_max_speed_flock * abs(flock[i].getPosition().y - flock[j].getPosition().y)/m_max_vision_flock);
                num_neighbours[i]++;
                num_neighbours[j]++;
            }
        }
    }
    for (int i = 0; i < flock.size(); i++)
    {
        if (num_neighbours[i] > 0)
            flock[i].velocity += vel_neighbours[i]/(float)num_neighbours[i] * matching_factor;
    }
}

void Flock::avoid_others(const float avoiding_factor, const float min_distance)
{
    for (int i = 0; i < flock.size(); i++)
    {
        for (int j = i+1; j < flock.size(); j++)
        {
            float distance_sqr = (flock[i].getPosition().x-flock[j].getPosition().x) * (flock[i].getPosition().x-flock[j].getPosition().x) + (flock[i].getPosition().y-flock[j].getPosition().y) * (flock[i].getPosition().y-flock[j].getPosition().y);
            if (distance_sqr <= min_distance * min_distance)
            {
                const sf::Vector2f tmp = (flock[j].getPosition() - flock[i].getPosition())/min_distance * avoiding_factor;
                flock[i].velocity -= tmp;
                flock[j].velocity += tmp;
            }
        }
    }
}

void Flock::hunt_flock(const float hunting_factor)
{
    sf::Vector2f hunt_neighbours(0, 0);
    int num_neighbours = 0;
    for (Boid& boid : flock)
    {
        float distance_sqr = (boid.getPosition().x-predator.getPosition().x) * (boid.getPosition().x-predator.getPosition().x) + (boid.getPosition().y-predator.getPosition().y) * (boid.getPosition().y-predator.getPosition().y);
        if (distance_sqr <= m_max_vision_predator * m_max_vision_predator)
        {
            hunt_neighbours += (boid.getPosition() - predator.getPosition())/(float)m_max_vision_predator;
            num_neighbours++;
        }
    }
    if (num_neighbours > 0)
        predator.velocity += hunt_neighbours/(float)num_neighbours * hunting_factor;
}

void Flock::avoid_predator(const float avoiding_factor)
{
    for (int i = 0; i < flock.size(); i++)
    {
        float distance_sqr = (flock[i].getPosition().x-predator.getPosition().x) * (flock[i].getPosition().x-predator.getPosition().x) + (flock[i].getPosition().y-predator.getPosition().y) * (flock[i].getPosition().y-predator.getPosition().y);
        if (distance_sqr <= m_max_vision_flock * m_max_vision_flock)
        {
            flock[i].velocity -= (predator.getPosition() - flock[i].getPosition())/m_max_vision_flock * avoiding_factor;
        }
    }
}
