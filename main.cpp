#include "System.h"

int main()
{
    System system({1000, 800},
                  SimulationSettings(
                          true,
                          false,
                          COLLISION_MODE::FIRST,
                          true,
                          false));

    system.AddParticles(200, 1, {2, 10});
    system.AddParticles(10, 1, {20, 1000});
//    system.AddParticle({100, 100}, {1, 0}, {100, 1000});
//    system.AddParticle({700, 101}, {-1, 0}, {100, 1000});
    system.ShowSimulationWindow();
}
