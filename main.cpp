#include <iostream>
#include "System.h"
#include "Sfml.h"

int main()
{
    System system({1000, 800});
    SimulationWindow simulation_window(system);


    system.AddParticles(1000, 1, {1, 1});

    simulation_window.ShowSimulationWindow();
//    system.AddParticles(1, 1, {20, 40});
//    system.AddParticle({30, 101}, {1, 0}, {10, 2});
//    system.AddParticle({200, 100}, {0, 0}, {10, 1});


//    sf::RenderWindow window(sf::VideoMode(unsigned(system.GetSystemSize().x), unsigned(system.GetSystemSize().y)), "SFML works!");
//    sf::RectangleShape BlackBackground({float(system.GetSystemSize().x), float(system.GetSystemSize().y)});
//    BlackBackground.setFillColor((sf::Color::Black));
//
//    while(window.isOpen()) {
//        window.clear();
//        sf::Event event{};
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        vector<sf::CircleShape> shapes;
//        for(auto& particle : system.GetParticles())
//        {
//            sf::CircleShape shape;
//            shape.setRadius(float(particle.properties.radius));
//            shape.setPosition(float(particle.position.x - particle.properties.radius), float(particle.position.y - particle.properties.radius));
//            window.draw(shape);
//        }
//
//        system.OperatorMove();
//        system.OperatorCollideWithBorder();
//        system.OperatorCollideWithParticle(MODE::SECOND);
////        system.OperatorCollideWithParticleComplexityNSquaredDividedByM(20);
////        system.OperatorCollideWithParticleComplexityNSquared();
////        system.OperatorGravity();
////        cout << "energy: " << system.OperatorComputeEnergy() << endl;
//        cout << "average number of collisions: " << system.AverageNumberOfCollisions() << endl;
//
//        window.display();
//    }

}
