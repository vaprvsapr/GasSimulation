#include <iostream>
#include "System.h"


int main()
{
    System system({1000, 800}, MODE::FIRST);

    system.AddParticles(1000, 1, {5, 1});

    system.ShowSimulationWindow();
}
