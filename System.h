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
#include <iostream>


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
    MODE mode;

    vector<Particle> particles = {};
private:
    sf::RenderWindow window;
    sf::RectangleShape black_background;

private:
    pair<bool, bool> IsContactWithBorder(Particle& _particle) const;

    static bool IsContactWithParticle(Particle& lhs, Particle& rhs);

    void OperatorCollideWithBorder();

    void OperatorCollideWithParticle();

    void OperatorMove();

    double OperatorComputeEnergy() const;

    void OperatorGravity();

    void CollideParticles(Particle& lhs, Particle& rhs);

    void FirstMethod();

    void SecondMethod(unsigned M);

    void ThirdMethod();

public:
    explicit System(Vec2D _system_size, MODE _mode);

    void AddParticle(Vec2D _position, Vec2D _velocity, Properties _properties);

    void AddParticles(
            unsigned _number_of_particles,
            double _initial_velocity,
            Properties _properties);

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

            OperatorCollideWithBorder();
            OperatorCollideWithParticle();
            OperatorMove();

            cout << "Energy: " << OperatorComputeEnergy() << endl;
            window.display();
//            cout << "average number of collisions: " << AverageNumberOfCollisions() << endl;

            if(epoch % 1000 == 0)
            {
                cout << "1000 epochs last for " << (clock() - time) / 1000<< " ms." << endl;
                time = clock();
            }
        }
    }
};


#endif //GASSIMULATION_SYSTEM_H
