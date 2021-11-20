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
    Vec2D size{};
    unsigned number_of_particles = 0;

    vector<Particle> particles = {};

public:
    explicit System(Vec2D _size)
    {

    }

    void AddParticles(
            unsigned _number_of_particles,
            double _initial_velocity,
            Properties _properties)
    {

    }

    bool IsContactWithParticle(Particle& lhs, Particle& rhs) const
    {
        return (sqrt(
                pow(lhs.position.x - rhs.position.x, 2) +
                pow(lhs.position.y - rhs.position.y, 2)) <
                (lhs.properties.radius + rhs.properties.radius));
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

    pair<bool, bool> IsContactWithBorder(Particle& _particle) const
    {
        bool is_contact_with_horizontal_boarder =
                (_particle.position.y < _particle.properties.radius) or
                (_particle.position.y > size.y - _particle.properties.radius);
        bool is_contact_with_vertical_boarder =
                (_particle.position.x < _particle.properties.radius) or
                (_particle.position.x > size.x - _particle.properties.radius);
        return make_pair(is_contact_with_horizontal_boarder, is_contact_with_vertical_boarder);
    }

    void OperatorCollisionWithBorder(Particle& _particle)
    {
        pair<bool, bool> is_contact_with_border = IsContactWithBorder(_particle);
        if(is_contact_with_border.first)
            _particle.velocity.y *= -1;
        if(is_contact_with_border.second)
            _particle.velocity.x *= -1;
    }


};


#endif //GASSIMULATION_SYSTEM_H
