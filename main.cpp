#include "System.h"

int main()
{
    System system({5000, 5000},
                  SimulationSettings(
                          false,
                          true,
                          COLLISION_MODE::THIRD,
                          true,
                          false));

    system.AddParticles(20000, 1, {1, 10});

//    system.AddParticle({100, 100}, {1, 0}, {100, 1000});
//    system.AddParticle({700, 101}, {-1, 0}, {100, 1000});
    system.RunSimulation();
}
