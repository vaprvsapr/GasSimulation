//
// Created by nikita on 17.11.2021.
//

#ifndef GASSIMULATION_SYSTEM_H
#define GASSIMULATION_SYSTEM_H

#include "Particle.h"
#include <vector>
#include <random>
#include <algorithm>
#include <set>
using namespace std;

enum class MODE {
    FIRST,
    SECOND,
    THIRD
};

class System {
private:
    Vec2D system_size{};
    unsigned number_of_particles = 0;
    unsigned long long number_of_collisions_between_particles = 0;
    double delta_time = 0.1;

    vector<Particle> particles = {};


    pair<bool, bool> IsContactWithBorder(Particle& _particle) const
    {
        bool is_contact_with_horizontal_boarder =
                (_particle.position.y < _particle.properties.radius) or
                (_particle.position.y > system_size.y - _particle.properties.radius);
        bool is_contact_with_vertical_boarder =
                (_particle.position.x < _particle.properties.radius) or
                (_particle.position.x > system_size.x - _particle.properties.radius);
        return make_pair(is_contact_with_horizontal_boarder, is_contact_with_vertical_boarder);
    }

    static bool IsContactWithParticle(Particle& lhs, Particle& rhs)
    {
        return (sqrt(
                pow(lhs.position.x - rhs.position.x, 2) +
                pow(lhs.position.y - rhs.position.y, 2)) <=
                (lhs.properties.radius + rhs.properties.radius));
    }
public:
    explicit System(Vec2D _system_size)
    {
        system_size = _system_size;
    }

    void AddParticle(Vec2D _position, Vec2D _velocity, Properties _properties)
    {
        particles.emplace_back(_position, _velocity, _properties);
    }

    void AddParticles(
            unsigned _number_of_particles,
            double _initial_velocity,
            Properties _properties)
    {
        unsigned counter = 0;

        while (counter < _number_of_particles)
        {
            double position_x = drand48() * (system_size.x - (2 * _properties.radius)) + _properties.radius;
            double position_y = drand48() * (system_size.y - (2 * _properties.radius)) + _properties.radius;

            double velocity_x = drand48() * (_initial_velocity);
            double velocity_y = (drand48() > 0.5 ? 1 : -1 ) *
                    sqrt(_initial_velocity * _initial_velocity -
                    velocity_x * velocity_x);

            Particle new_particle({position_x, position_y},
                                  {velocity_x, velocity_y},
                                  _properties);

            bool no_contact = true;
            for(auto& particle : particles)
            {
                if(IsContactWithParticle(new_particle, particle))
                {
                    no_contact = false;
                    break;
                }
            }

            if(no_contact) {
                particles.push_back(new_particle);
                counter++;
//                cout << particles.size() << " " << position_x << " " << position_y << endl; // uncomment if you want to see process of adding particles
            }
        }
    }


    void CollideWithParticle(Particle& lhs, Particle& rhs)
    {
        if(IsContactWithParticle(lhs, rhs))
        {
            number_of_collisions_between_particles++;

            Vec2D lhs_perpendicular = lhs.velocity - ProjectionVec2D(lhs.velocity, ConnectVec2D(lhs.position, rhs.position));
            Vec2D lhs_parallel = lhs.velocity - lhs_perpendicular;
            Vec2D rhs_perpendicular = rhs.velocity - ProjectionVec2D(rhs.velocity, ConnectVec2D(rhs.position, lhs.position));
            Vec2D rhs_parallel = rhs.velocity - rhs_perpendicular;
            Vec2D velocity_C = (lhs_parallel * lhs.properties.mass + rhs_parallel * rhs.properties.mass) / (lhs.properties.mass + rhs.properties.mass);
            Vec2D lhs_parallel_relative = lhs_parallel - velocity_C;
            Vec2D rhs_parallel_relative = rhs_parallel - velocity_C;
            lhs_parallel = rhs_parallel_relative * rhs.properties.mass / lhs.properties.mass + velocity_C;
            rhs_parallel = lhs_parallel_relative * lhs.properties.mass / rhs.properties.mass + velocity_C;
            lhs.velocity = lhs_parallel + lhs_perpendicular;
            rhs.velocity = rhs_parallel + rhs_perpendicular;


//            Vec2D new_lhs_velocity = lhs.velocity -
//                                     ProjectionVec2D(lhs.velocity, ConnectVec2D(lhs.position, rhs.position)) +
//                                     ProjectionVec2D(rhs.velocity, ConnectVec2D(rhs.position, lhs.position));
//            Vec2D new_rhs_velocity = rhs.velocity -
//                                     ProjectionVec2D(rhs.velocity, ConnectVec2D(rhs.position, lhs.position)) +
//                                     ProjectionVec2D(lhs.velocity, ConnectVec2D(lhs.position, rhs.position));
//
//            lhs.velocity = new_lhs_velocity;
//            rhs.velocity = new_rhs_velocity;
        }
    }

    void CollideWithBorder(Particle& _particle)
    {
        pair<bool, bool> is_contact_with_border = IsContactWithBorder(_particle);
        if(is_contact_with_border.first)
            _particle.velocity.y *= -1;
        if(is_contact_with_border.second)
            _particle.velocity.x *= -1;
    }

    [[nodiscard]] Vec2D GetSystemSize() const
    {
        return system_size;
    }

    [[nodiscard]] vector<Particle> GetParticles() const
    {
        return particles;
    }

    void OperatorMove()
    {
        for(auto& particle : particles)
        {
            particle.position = {particle.position.x + particle.velocity.x * delta_time, particle.position.y + particle.velocity.y * delta_time};
        }
    }

    void OperatorCollideWithBorder()
    {
        for(auto& particle : particles)
        {
            CollideWithBorder(particle);
        }
    }

    void OperatorCollideWithParticle(MODE mode)
    {
        switch (mode)
        {
            case MODE::FIRST:
                OperatorCollideWithParticleComplexityNSquared();
                break;
            case MODE::SECOND:
                OperatorCollideWithParticleComplexityNSquaredDividedByM(20);
                break;
            case MODE::THIRD:
                break;
        }
    }

    void OperatorCollideWithParticleComplexityNSquared()
    {
        for(int i = 0; i < particles.size() - 1; i++)
        {
            for(int j = i + 1; j < particles.size(); j++)
            {
                CollideWithParticle(particles[i], particles[j]);
            }
        }
    }

    void OperatorCollideWithParticleComplexityNSquaredDividedByM(unsigned M) // NEEDS CORRECTION
    {
        vector<vector<vector<Particle>>> grid;

        grid.resize(M);
        for(auto& column : grid)
            column.resize(M);

        double delta_x = system_size.x / M;
        double delta_y = system_size.y / M;

        for(auto& particle : particles)
        {
            grid[floor(particle.position.x / delta_x)][floor(particle.position.y / delta_y)].push_back(particle);
        }

        for(auto& column : grid)
        {
            for(auto& element : column)
            {
                for(int i = 0; i < int(element.size()) - 1; i++)
                {
                    for(int j = i + 1; j < element.size(); j++)
                    {
                        CollideWithParticle(element[i], element[j]);
                    }
                }
            }
        }
        particles.clear();
        for(auto& column : grid)
            for(auto& element : column)
                for(auto& particle : element)
                    particles.push_back(particle);
    }

    void OperatorGravity()
    {
        for(auto& particle : particles)
        {
            particle.velocity.y += delta_time * 10;
        }
    }

    double OperatorComputeEnergy()
    {
        double energy = 0;
        for(auto& particle : particles)
        {
            energy += (pow(particle.velocity.x, 2) + pow(particle.velocity.y, 2)) * particle.properties.mass / 2;
        }
        return energy;
    }
    double AverageNumberOfCollisions()
    {
        return (double(number_of_collisions_between_particles) / double(particles.size()));
    }
};


#endif //GASSIMULATION_SYSTEM_H
