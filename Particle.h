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
             Properties _properties);
};

bool operator==(Particle lhs, Particle rhs);


#endif //GASSIMULATION_PARTICLE_H
