//
// Created by nikita on 01.12.2021.
//

#include "Particle.h"

bool operator==(const Particle lhs, const Particle rhs)
{
    return lhs.position == rhs.position and lhs.velocity == rhs.velocity and lhs.properties == rhs.properties;
}

Particle::Particle(Vec2D _position, Vec2D _velocity, Properties _properties)
{
    position = _position;
    velocity = _velocity;
    properties = _properties;
}