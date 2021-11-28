#include <iostream>
#include "System.h"


int main()
{
    System system({1000, 800});


    system.AddParticles(100, 1, {10, 1});

    system.ShowSimulationWindow();
}
