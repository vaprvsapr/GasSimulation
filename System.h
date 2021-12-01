//
// Created by nikita on 17.11.2021.
//

#ifndef GASSIMULATION_SYSTEM_H
#define GASSIMULATION_SYSTEM_H

#include <vector>
#include <random>
#include <algorithm>
#include <set>
#include <iostream>

#include "SFML/Graphics.hpp"
#include "Particle.h"

using namespace std;

enum class COLLISION_MODE{
    DISABLED,
    FIRST,
    SECOND,
    THIRD
};

struct SimulationSettings {
    bool show_simulation_window;
    bool show_diagram;
    bool collide_with_border;
    bool gravity;
    COLLISION_MODE collision_mode;

    explicit SimulationSettings(bool _show_simulation_window = true,
                       bool _show_diagram = true,
                       COLLISION_MODE _collision_mode = COLLISION_MODE::FIRST,
                       bool _collide_with_border = true,
                       bool _gravity = false )
    {
        show_simulation_window = _show_simulation_window;
        show_diagram = _show_diagram;
        collide_with_border = _collide_with_border;
        gravity = _gravity;
        collision_mode = _collision_mode;
    }
};

class System {
private:
    Vec2D system_size{};
    unsigned number_of_particles = 0;
    unsigned long long number_of_collisions_between_particles = 0;
    double delta_time = 0.1;
    SimulationSettings simulation_settings;

    vector<Particle> particles = {};
private:
    sf::RenderWindow simulation_window;
    sf::RectangleShape black_background;

private:
    pair<bool, bool> IsContactWithBorder(Particle& _particle) const;
    static bool IsContactWithParticle(Particle& lhs, Particle& rhs);
    void CollideParticles(Particle& lhs, Particle& rhs);
    void FirstMethod();
    void SecondMethod(unsigned M);
    void ThirdMethod();
    void OperatorCollideWithBorder();
    void OperatorCollideParticles();
    void OperatorMove();
    double OperatorComputeEnergy() const;
    void OperatorGravity();
public:
    explicit System(Vec2D _system_size, SimulationSettings _simulation_settings);

    void AddParticle(Vec2D _position, Vec2D _velocity, Properties _properties);

    void AddParticles(unsigned _number_of_particles, double _initial_velocity, Properties _properties);

    double AverageNumberOfCollisions();

    void RunSimulation()
    {
        // to be implemented
    }


    void ShowSimulationWindow()
    {
        int epoch = 0;
        clock_t time = clock();
        while(simulation_window.isOpen())
        {
            epoch++;
            simulation_window.clear();
            sf::Event event{};
            while(simulation_window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                    simulation_window.close();
            }

            for(auto& particle : particles)
            {
                sf::CircleShape circle_shape;
                circle_shape.setRadius(float(particle.properties.radius));
                circle_shape.setPosition(
                        float(particle.position.x - particle.properties.radius),
                        float(particle.position.y - particle.properties.radius));
                simulation_window.draw(circle_shape);
            }

            OperatorCollideWithBorder();
            OperatorCollideParticles();
            OperatorMove();

            cout << "Energy: " << OperatorComputeEnergy() << endl;
            simulation_window.display();
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
