//
// Created by nikita on 01.12.2021.
//

#include "System.h"



System::System(Vec2D _system_size, SimulationSettings _simulation_settings):
        simulation_settings(_simulation_settings)
{
    system_size = _system_size;
}

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
    max_abs_velocity = 0;
    for(auto& particle : particles)
    {
        if(sqrt(particle.velocity.SquareLengthVec2D()) > max_abs_velocity)
            max_abs_velocity = sqrt(particle.velocity.SquareLengthVec2D());
        particle.position = {particle.position.x + particle.velocity.x * delta_time, particle.position.y + particle.velocity.y * delta_time};
        particle.collided = false;
    }
    if(simulation_settings.adaptive_time)
        delta_time = 0.5 / max_abs_velocity;
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

void System::AddParticlesInitial(unsigned int _number_of_particles,
                                 double _initial_velocity,
                                 Properties _properties)
{

    unsigned grid_size = ceil(sqrt(_number_of_particles));
    double vertical_grid_interval = system_size.y / (grid_size + 1);
    double horizontal_grid_interval = system_size.x / (grid_size + 1);

    for(unsigned i = 1; i < grid_size + 1; i++)
    {
        for(unsigned j = 1; j < grid_size + 1; j++)
        {
            double position_x = j * horizontal_grid_interval;
            double position_y = i * vertical_grid_interval;

            double velocity_x = drand48() * (_initial_velocity);
            double velocity_y = (drand48() > 0.5 ? 1 : -1 ) *
                                sqrt(_initial_velocity * _initial_velocity -
                                     velocity_x * velocity_x);

            Particle new_particle({position_x, position_y},
                                  {velocity_x, velocity_y},
                                  _properties);

            particles.push_back(new_particle);
        }
    }

}

double System::AverageNumberOfCollisions()
{
    return (double(number_of_collisions_between_particles) / double(particles.size()));
}

void System::RunSimulation()
{
    if(!simulation_settings.show_simulation_window.first and !simulation_settings.show_diagram)
    {

        auto time = clock();
        while(true)
        {
            if(epoch == 100)
                cout << "time for 100 epochs and for " << particles.size() << " particles : " << (clock() - time) / 1000 << "ms" << endl;
            epoch++;
            OperatorMove();
            if(simulation_settings.collide_with_border)
                OperatorCollideWithBorder();
            OperatorCollideParticles();

            cout << "epoch: " << epoch << endl;
        }
    }


    if(simulation_settings.show_simulation_window.first and !simulation_settings.show_diagram)
    {

        sf::RenderWindow simulation_window(sf::RenderWindow(sf::VideoMode(
                                                                    unsigned(system_size.x),
                                                                    unsigned(system_size.y)),
                                                            "Simulation simulation_window."));
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

            cout << "Delta time: " << delta_time << endl;
        }
    }

    if(simulation_settings.show_diagram and !simulation_settings.show_simulation_window.first)
    {
        const int number_of_columns = simulation_settings.show_simulation_window.second;
        Vec2D distribution_window_size{500, 500};
        sf::RenderWindow distribution_window(sf::RenderWindow(sf::VideoMode(
                                                                    unsigned(distribution_window_size.x),
                                                                    unsigned(distribution_window_size.y)),
                                                            "Velocity distribution."));
        sf::RectangleShape shapes_of_columns[number_of_columns];
        while(distribution_window.isOpen())
        {
            epoch++;

            sf::Event event{};
            while(distribution_window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                    distribution_window.close();
            }

            int data[number_of_columns];
            {
                for(auto& element : data)
                    element = 0;

                double velocity_interval = max_abs_velocity / number_of_columns;

                for(auto& particle : particles)
                {
                    data[static_cast<unsigned>(floor(sqrt(particle.velocity.SquareLengthVec2D()) / velocity_interval))]++;
                }

                int max_number = 0;
                for(auto& element : data)
                    max_number = max(max_number, element);

                double vertical_interval = distribution_window_size.y / max_number;
                double horizontal_interval = distribution_window_size.x / static_cast<double>(number_of_columns);

                for(int index = 0; index < number_of_columns; index++)
                {
                    sf::RectangleShape column;
                    column.setSize({static_cast<float>(horizontal_interval),
                                    static_cast<float>(data[index] * vertical_interval)});
                    column.setPosition(static_cast<float>(horizontal_interval * index),
                                       static_cast<float>(distribution_window_size.y - data[index] * vertical_interval));
                    shapes_of_columns[index] = column;
                }

                distribution_window.clear();
                for(auto& column_shape : shapes_of_columns)
                    distribution_window.draw(column_shape);
                distribution_window.display();

                cout << "epoch: " << epoch << endl;
            }


            OperatorCollideWithBorder();
            OperatorCollideParticles();
            OperatorMove();
        }
    }

    if(simulation_settings.show_diagram and simulation_settings.show_simulation_window.first)
    {
        const int number_of_columns = simulation_settings.show_simulation_window.second;
        Vec2D distribution_window_size{500, 500};
        sf::RenderWindow distribution_window(sf::RenderWindow(sf::VideoMode(
                                                                      unsigned(distribution_window_size.x),
                                                                      unsigned(distribution_window_size.y)),
                                                              "Velocity distribution."));
        sf::RenderWindow simulation_window(sf::RenderWindow(sf::VideoMode(
                                                                    unsigned(system_size.x),
                                                                    unsigned(system_size.y)),
                                                            "Simulation simulation_window."));

        sf::RectangleShape shapes_of_columns[number_of_columns];
        while(distribution_window.isOpen() and simulation_window.isOpen())
        {
            epoch++;

            sf::Event event{};
            while(distribution_window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    distribution_window.close();
                    simulation_window.close();
                }

            }
            while(simulation_window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    distribution_window.close();
                    simulation_window.close();
                }
            }

            int data[number_of_columns];
            {
                for(auto& element : data)
                    element = 0;

                double velocity_interval = max_abs_velocity / number_of_columns;

                for(auto& particle : particles)
                {
                    data[static_cast<unsigned>(floor(sqrt(particle.velocity.SquareLengthVec2D()) / velocity_interval))]++;
                }

                int max_number = 0;
                for(auto& element : data)
                    max_number = max(max_number, element);

                double vertical_interval = distribution_window_size.y / max_number;
                double horizontal_interval = distribution_window_size.x / static_cast<double>(number_of_columns);

                for(int index = 0; index < number_of_columns; index++)
                {
                    sf::RectangleShape column;
                    column.setSize({static_cast<float>(horizontal_interval),
                                    static_cast<float>(data[index] * vertical_interval)});
                    column.setPosition(static_cast<float>(horizontal_interval * index),
                                       static_cast<float>(distribution_window_size.y - data[index] * vertical_interval));
                    shapes_of_columns[index] = column;
                }

                distribution_window.clear();
                for(auto& column_shape : shapes_of_columns)
                    distribution_window.draw(column_shape);
                distribution_window.display();

                cout << "epoch: " << epoch << endl;
            }

            simulation_window.clear();
            for(auto& particle : particles)
            {
                sf::CircleShape circle_shape;
                circle_shape.setRadius(float(particle.properties.radius));
                circle_shape.setPosition(
                        float(particle.position.x - particle.properties.radius),
                        float(particle.position.y - particle.properties.radius));
                simulation_window.draw(circle_shape);
            }
            simulation_window.display();

            OperatorCollideWithBorder();
            OperatorCollideParticles();
            OperatorMove();

        }

    }
}

unsigned System::Epoch() const {
    return epoch;
}
