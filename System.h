//
// Created by nikita on 17.11.2021.
//

#ifndef GASSIMULATION_SYSTEM_H
#define GASSIMULATION_SYSTEM_H

#include "SFML/Graphics.hpp"
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
private:
    sf::RenderWindow window;
    sf::RectangleShape black_background;

private:
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
    explicit System(Vec2D _system_size):
            window(
                    sf::RenderWindow(sf::VideoMode(
                                    unsigned(_system_size.x),
                                    unsigned(_system_size.y)),
                                    "Simulation window.")),
            black_background({float(_system_size.x), float(_system_size.y)})
    {
        system_size = _system_size;
        black_background.setFillColor((sf::Color::Black));
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
                cout << particles.size() << " " << position_x << " " << position_y << endl; // uncomment if you want to see process of adding particles
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

//            Vec2D new_lhs_velocity = lhs.velocity -
//                                     ProjectionVec2D(lhs.velocity, ConnectVec2D(lhs.position, rhs.position)) +
//                                     ProjectionVec2D(rhs.velocity, ConnectVec2D(rhs.position, lhs.position));
//            Vec2D new_rhs_velocity = rhs.velocity -
//                                     ProjectionVec2D(rhs.velocity, ConnectVec2D(rhs.position, lhs.position)) +
//                                     ProjectionVec2D(lhs.velocity, ConnectVec2D(lhs.position, rhs.position));

            lhs.velocity = lhs_parallel + lhs_perpendicular;
            rhs.velocity = rhs_parallel + rhs_perpendicular;

//            cout << "vx: " << lhs.velocity.x << ", vy: " << lhs.velocity.y << "; nvx: " << new_lhs_velocity.x << ", nvy: " << new_lhs_velocity.y << endl;
//            lhs.velocity = new_lhs_velocity;
//            rhs.velocity = new_rhs_velocity;

            lhs.collided = true;
            rhs.collided = true;
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
            particle.collided = false;
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
                OperatorCollideWithParticle();
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

    void OperatorCollideWithParticle()
    {
        sort(particles.begin(), particles.end(), [](Particle& lhs, Particle& rhs){
            return lhs.position.x < rhs.position.x;
        });

        int particles_size = int(particles.size());
        double radius = particles[0].properties.radius;
        double delta;
        for(int i = 0; i < particles_size; i++)
        {
            if(particles[i].collided)
                continue;

            delta = 0;
            for(int j = i - 1; j >= 0 and delta < radius; j--)
            {
                CollideWithParticle(particles[i], particles[j]);
                delta  = particles[i].position.x - particles[j].position.x;
            }
            delta = 0;
            for(int j = i + 1; j < particles_size and delta < radius; j++)
            {
                CollideWithParticle(particles[i], particles[j]);
                delta = particles[j].position.x - particles[i].position.x;
            }
        }
        sort(particles.begin(), particles.end(), [](Particle& lhs, Particle& rhs){
            return lhs.position.y < rhs.position.y;
        });
        for(int i = 0; i < particles_size; i++)
        {
            if(particles[i].collided)
                continue;

            delta = 0;
            for(int j = i - 1; j >= 0 and delta < radius; j--)
            {
                CollideWithParticle(particles[i], particles[j]);
                delta = particles[i].position.y - particles[j].position.y;
            }
            for(int j = i + 1; j < particles_size and delta < radius; j++)
            {
                CollideWithParticle(particles[i], particles[j]);
                delta = particles[j].position.y - particles[i].position.y;
            }
        }
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
            energy += particle.velocity.Modulus() * particle.properties.mass / 2;
        }
        return energy;
    }
    double AverageNumberOfCollisions()
    {
        return (double(number_of_collisions_between_particles) / double(particles.size()));
    }

    void ShowSimulationWindow()
    {
        int epoch = 0;
        clock_t time = clock();
        while(window.isOpen())
        {
            epoch++;
            window.clear();
            sf::Event event{};
            while(window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                    window.close();
            }

            for(auto& particle : particles)
            {
                sf::CircleShape circle_shape;
                circle_shape.setRadius(float(particle.properties.radius));
                circle_shape.setPosition(
                        float(particle.position.x - particle.properties.radius),
                        float(particle.position.y - particle.properties.radius));
                window.draw(circle_shape);
            }

            OperatorMove();
            OperatorCollideWithBorder();
            OperatorCollideWithParticle(MODE::SECOND);
            window.display();
            // cout << "average number of collisions: " << AverageNumberOfCollisions() << endl;

            if(epoch % 1000 == 0){
                cout << "1000 epochs last for " << (clock() - time) / 1000<< " ms." << endl;
                time = clock();
            }
        }
    }
};


#endif //GASSIMULATION_SYSTEM_H
