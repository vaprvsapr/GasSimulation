//
// Created by nikita on 17.11.2021.
//

#ifndef GASSIMULATION_SYSTEM_H
#define GASSIMULATION_SYSTEM_H

#include "Particle.h"
#include <vector>
#include <random>
using namespace std;

class System {
private:
    Vec2D system_size{};
    unsigned number_of_particles = 0;

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
                pow(lhs.position.y - rhs.position.y, 2)) <
                (lhs.properties.radius + rhs.properties.radius));
    }
public:
    explicit System(Vec2D _system_size)
    {
        system_size = _system_size;
    }

    void AddParticles(
            unsigned _number_of_particles,
            double _initial_velocity,
            Properties _properties)
    {
        unsigned counter = 0;

        unsigned initial_number_of_particles_in_the_system = particles.size();

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
            //cout << "-";
            if(no_contact) {
                particles.push_back(new_particle);
                counter++;
                cout << particles.size() << " " << position_x << " " << position_y << endl; // uncomment if you want to see process of adding particles
            }
        }
        //cout << "Successfully added: " << particles.size() - initial_number_of_particles_in_the_system << " particles." << endl;
    }


    void OperatorCollisionWithParticle(Particle& lhs, Particle& rhs)
    {
        Vec2D new_lhs_velocity = lhs.velocity -
                ProjectionVec2D(lhs.velocity, ConnectVec2D(lhs.position, rhs.position)) +
                ProjectionVec2D(rhs.velocity, ConnectVec2D(rhs.position, lhs.position));
        Vec2D new_rhs_velocity = rhs.velocity -
                ProjectionVec2D(rhs.velocity, ConnectVec2D(rhs.position, lhs.position)) +
                ProjectionVec2D(lhs.velocity, ConnectVec2D(lhs.position, rhs.position));

        lhs.velocity = new_lhs_velocity;
        rhs.velocity = new_rhs_velocity;
    }

    void OperatorCollisionWithBorder(Particle& _particle)
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
};


#endif //GASSIMULATION_SYSTEM_H
