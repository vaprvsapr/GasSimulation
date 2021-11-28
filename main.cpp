#include <iostream>
#include "System.h"


int main()
{
    System system({1000, 800});

    system.AddParticles(10000, 1, {1, 1});

    system.ShowSimulationWindow();
}
