//
// Created by nikita on 24.11.2021.
//

#ifndef MAIN_CPP_SFML_H
#define MAIN_CPP_SFML_H

#include "SFML/Graphics.hpp"
#include "System.h"

class SimulationWindow {
private:
    sf::RenderWindow window;
    sf::RectangleShape black_background;
    System& system;

public:
    explicit SimulationWindow(System &_system) :
    system(_system),
    window(sf::RenderWindow(sf::VideoMode(
            unsigned(_system.GetSystemSize().x),
            unsigned(_system.GetSystemSize().y)),
                            "Simulation window.")),
    black_background({float(_system.GetSystemSize().x),
                      float(_system.GetSystemSize().y)})
    {
        black_background.setFillColor((sf::Color::Black));
    }

    void ShowSimulationWindow()
    {
        while(window.isOpen())
        {
            window.clear();
            sf::Event event{};
            while(window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                    window.close();
            }

            for(auto& particle : system.GetParticles())
            {
                sf::CircleShape circle_shape;
                circle_shape.setRadius(float(particle.properties.radius));
                circle_shape.setPosition(
                        float(particle.position.x - particle.properties.radius),
                        float(particle.position.y - particle.properties.radius));
                window.draw(circle_shape);
            }

            system.OperatorMove();
            system.OperatorCollideWithBorder();
            system.OperatorCollideWithParticle(MODE::SECOND);
            window.display();
        }
    }
};

#endif //MAIN_CPP_SFML_H
