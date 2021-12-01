//
// Created by nikita on 01.12.2021.
//

#include "System.h"

pair<bool, bool> System::IsContactWithBorder(Particle &_particle) const
{
    bool is_contact_with_horizontal_boarder =
            (_particle.position.y < _particle.properties.radius) or
            (_particle.position.y > system_size.y - _particle.properties.radius);
    bool is_contact_with_vertical_boarder =
            (_particle.position.x < _particle.properties.radius) or
            (_particle.position.x > system_size.x - _particle.properties.radius);
    return make_pair(is_contact_with_horizontal_boarder, is_contact_with_vertical_boarder);
}

bool System::IsContactWithParticle(Particle &lhs, Particle &rhs)
{
    return (sqrt(
            pow(lhs.position.x - rhs.position.x, 2) +
            pow(lhs.position.y - rhs.position.y, 2)) <=
            (lhs.properties.radius + rhs.properties.radius));
}

void System::OperatorCollideWithBorder()
{
    for(auto& particle : particles)
    {
        pair<bool, bool> is_contact_with_border = IsContactWithBorder(particle);
        if(is_contact_with_border.first)
            particle.velocity.y *= -1;
        if(is_contact_with_border.second)
            particle.velocity.x *= -1;
    }
}

void System::OperatorCollideParticles()
{
    switch (simulation_settings.collision_mode)
    {
        case COLLISION_MODE::FIRST:
            FirstMethod();
            break;
        case COLLISION_MODE::SECOND:
            SecondMethod(20);
            break;
        case COLLISION_MODE::THIRD:
            ThirdMethod();
            break;
        case COLLISION_MODE::DISABLED:
            break;
    }
}

void System::CollideParticles(Particle &lhs, Particle &rhs)
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

double System::OperatorComputeEnergy() const
{
    double energy = 0;
    for(auto& particle : particles)
    {
        energy += particle.velocity.SquareLengthVec2D() * particle.properties.mass / 2;
    }
    return energy;
}

void System::OperatorMove()
{
    for(auto& particle : particles)
    {
        particle.position = {particle.position.x + particle.velocity.x * delta_time, particle.position.y + particle.velocity.y * delta_time};
        particle.collided = false;
    }
}

void System::OperatorGravity()
{
    for(auto& particle : particles)
    {
        particle.velocity.y += delta_time * 10;
    }
}

void System::FirstMethod()
{
    for(int i = 0; i < particles.size() - 1; i++)
    {
        for(int j = i + 1; j < particles.size(); j++)
        {
            CollideParticles(particles[i], particles[j]);
        }
    }
}

void System::SecondMethod(unsigned int M)
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
                    CollideParticles(element[i], element[j]);
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

void System::ThirdMethod()
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
            CollideParticles(particles[i], particles[j]);
            delta  = particles[i].position.x - particles[j].position.x;
        }
        delta = 0;
        for(int j = i + 1; j < particles_size and delta < radius; j++)
        {
            CollideParticles(particles[i], particles[j]);
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
            CollideParticles(particles[i], particles[j]);
            delta = particles[i].position.y - particles[j].position.y;
        }
        for(int j = i + 1; j < particles_size and delta < radius; j++)
        {
            CollideParticles(particles[i], particles[j]);
            delta = particles[j].position.y - particles[i].position.y;
        }
    }
}

System::System(Vec2D _system_size, SimulationSettings _simulation_settings):
        simulation_window(sf::RenderWindow(sf::VideoMode(
                                                   unsigned(_system_size.x),
                                                   unsigned(_system_size.y)),
                                           "Simulation simulation_window.")),
        black_background({float(_system_size.x), float(_system_size.y)}),
        simulation_settings(_simulation_settings)
{
    system_size = _system_size;
    black_background.setFillColor((sf::Color::Black));
}

void System::AddParticle(Vec2D _position, Vec2D _velocity, Properties _properties)
{
    particles.emplace_back(_position, _velocity, _properties);
}

void System::AddParticles(unsigned int _number_of_particles,
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

double System::AverageNumberOfCollisions()
{
    return (double(number_of_collisions_between_particles) / double(particles.size()));
}
