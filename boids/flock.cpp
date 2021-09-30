//
//  flock.cpp
//  boids
//
//  Created by Jonas Wattendorff on 29.09.21.
//

#include <cassert>

#include "flock.hpp"

Flock::Flock(const int& n, const int& max_speed, const int& max_vision)
    : m_max_speed(max_speed), m_max_vision(max_vision)
{
    srand(0);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            m_boids.push_back(Boid(rand() % 750, rand() % 750, rand() % 2*max_speed - max_speed, rand() % 2*max_speed - max_speed));
        }
    }
    m_adj_matrix.resize(pow(n, 2));
    for (int i = 0; i < pow(n, 2); ++i)
        m_adj_matrix[i].resize(pow(n, 2));
    fill_adj_matrix();
}

void Flock::move(const sf::Vector2u& window_size)
{
    fill_adj_matrix();
    for (Boid& boid : m_boids)
    {
        avoid_others_and_match_velocity(0.01, 25, 0.01);
        boid.limit_speed(m_max_speed);
        boid.avoid_bounds(window_size.x, window_size.y, 150, 0.5);
        boid.move_boid();
    }
}

void Flock::draw(sf::RenderWindow& window) const
{
    for (const Boid& boid : m_boids)
    {
        window.draw(boid);
    }
//    for (int i = 0; i < m_adj_matrix.size(); i++)
//    {
//        for (int j = i+1; j < m_adj_matrix.size(); j++)
//        {
//            if (m_adj_matrix[i][j] != 0)
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

void Flock::fill_adj_matrix()
{
    for (int i = 0; i < m_adj_matrix.size(); i++)
    {
        for (int j = i+1; j < m_adj_matrix.size(); j++)
        {
            float distance_sqr = pow(m_boids[i].getPosition().x-m_boids[j].getPosition().x, 2)
                                  + pow(m_boids[i].getPosition().y-m_boids[j].getPosition().y, 2);
            if (distance_sqr <= pow(m_max_vision, 2))
            {
                m_adj_matrix[i][j] = distance_sqr;
                m_adj_matrix[j][i] = distance_sqr;
            }
            else
            {
                m_adj_matrix[i][j] = 0;
                m_adj_matrix[j][i] = 0;
            }
        }
    }
}

//void Flock::match_velocity(const float& matching_factor)
//{
//    std::vector<sf::Vector2f> vel_neighbours(m_adj_matrix.size());
//    std::vector<int> num_neighbours(m_adj_matrix.size());
//    for (int i = 0; i < m_adj_matrix.size(); i++)
//    {
//        for (int j = 0; j < m_adj_matrix.size(); j++)
//        {
//            if (m_adj_matrix[i][j] != 0)
//            {
//                vel_neighbours[i] += sf::Vector2f(m_boids[j].velocity.x * abs(m_boids[j].getPosition().x - m_boids[i].getPosition().x)/m_max_vision, m_boids[j].velocity.y * abs(m_boids[j].getPosition().y - m_boids[i].getPosition().y)/m_max_vision);
//                num_neighbours[i]++;
//            }
//        }
//    }
//    for (int i = 0; i < m_adj_matrix.size(); i++)
//    {
//        if (num_neighbours[i] > 0)
//            m_boids[i].velocity += (vel_neighbours[i]/(float)num_neighbours[i]) * matching_factor;
//    }
//}

void Flock::avoid_others_and_match_velocity(const float& avoiding_factor, const float& min_distance, const float& matching_factor)
{
    std::vector<sf::Vector2f> ma_vel_neighbours(m_adj_matrix.size());
    std::vector<sf::Vector2f> av_vel_neighbours(m_adj_matrix.size());
    std::vector<int> num_neighbours(m_adj_matrix.size());
    for (int i = 0; i < m_adj_matrix.size(); i++)
    {
        for (int j = 0; j < m_adj_matrix.size(); j++)
        {
            if (m_adj_matrix[i][j] != 0)
            {
                ma_vel_neighbours[i] += sf::Vector2f(m_boids[j].velocity.x * abs(m_boids[j].getPosition().x - m_boids[i].getPosition().x)/m_max_vision, m_boids[j].velocity.y * abs(m_boids[j].getPosition().y - m_boids[i].getPosition().y)/m_max_vision);
                num_neighbours[i]++;
                
                if (m_adj_matrix[i][j] <= pow(min_distance, 2))
                {
                    float distance = pow(m_boids[j].getPosition().x - m_boids[i].getPosition().x, 2) + pow(m_boids[j].getPosition().y - m_boids[i].getPosition().y, 2);
                    av_vel_neighbours[i] -= (m_boids[j].getPosition() - m_boids[i].getPosition()) * avoiding_factor*min_distance/(distance+1);
                }
            }
        }
    }
    for (int i = 0; i < m_adj_matrix.size(); i++)
    {
        if (num_neighbours[i] > 0)
        {
            m_boids[i].velocity += ma_vel_neighbours[i]/(float)num_neighbours[i] * matching_factor;
            m_boids[i].velocity += av_vel_neighbours[i];
        }
    }
}
