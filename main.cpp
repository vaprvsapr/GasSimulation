#include "System.h"

int main()
{
    System system({3000, 3000},
                  SimulationSettings(
                          {false, 50},
                          true,
                          COLLISION_MODE::THIRD,
                          true,
                          false,
                          true));

    system.AddParticlesInitial(200'000, 0.1, {1, 1});


//    system.AddParticle({100, 100}, {1, 0}, {100, 1000});
//    system.AddParticle({700, 101}, {-1, 0}, {100, 1000});
    system.RunSimulation();
}
