#include "SFML/Graphics.hpp"
#include <iostream>
#include <cmath>
#include "System.h"



int main()
{
    System system({200, 200});
    system.AddParticles(50, 10, {10, 1});

    sf::RenderWindow window(sf::VideoMode(unsigned(system.GetSystemSize().x), unsigned(system.GetSystemSize().y)), "SFML works!");
    sf::RectangleShape BlackBackground({float(system.GetSystemSize().x), float(system.GetSystemSize().y)});
    BlackBackground.setFillColor((sf::Color::Black));

    for(auto particle : system.GetParticles())
    {
        cout << particle.position.x << " " << particle.position.y << endl;
        cout << particle.properties.radius << endl;
    }
    cout << system.GetSystemSize().x << " " << system.GetSystemSize().y << endl;

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

        window.display();
    }

}
