//
// Created by nikita on 17.11.2021.
//

#ifndef GASSIMULATION_PARTICLE_H
#define GASSIMULATION_PARTICLE_H

#include "Vec2D.h"
#include "Properties.h"

class Particle {
public:
    Vec2D position{};
    Vec2D velocity{};
    Properties properties{};
    bool collided = false;

    Particle(Vec2D _position,
             Vec2D _velocity,
             Properties _properties)
    {
        position = _position;
        velocity = _velocity;
        properties = _properties;
    }
};

bool operator==(const Particle lhs, const Particle rhs)
{
    return lhs.position == rhs.position and lhs.velocity == rhs.velocity and lhs.properties == rhs.properties;
}


#endif //GASSIMULATION_PARTICLE_H
