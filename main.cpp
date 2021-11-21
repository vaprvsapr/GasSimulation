#include "SFML/Graphics.hpp"
#include <iostream>
#include "System.h"



int main()
{
    System system({1000, 800});
    system.AddParticles(5000, 10, {2, 1});
//    system.AddParticle({20, 120}, {10, 0}, {20, 1});
//    system.AddParticle({100, 20}, {0, 10}, {20, 1});

    sf::RenderWindow window(sf::VideoMode(unsigned(system.GetSystemSize().x), unsigned(system.GetSystemSize().y)), "SFML works!");
    sf::RectangleShape BlackBackground({float(system.GetSystemSize().x), float(system.GetSystemSize().y)});
    BlackBackground.setFillColor((sf::Color::Black));

    while(window.isOpen()) {
        window.clear();
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        vector<sf::CircleShape> shapes;
        for(auto& particle : system.GetParticles())
        {
            sf::CircleShape shape;
            shape.setRadius(float(particle.properties.radius));
            shape.setPosition(float(particle.position.x - particle.properties.radius), float(particle.position.y - particle.properties.radius));
            window.draw(shape);
        }
        system.OperatorMove();
        system.OperatorCollideWithBorder();
        system.OperatorCollideWithParticleComplexityNSquared();
        cout << "energy: " << system.OperatorComputeEnergy() << endl;

        window.display();
    }

}
