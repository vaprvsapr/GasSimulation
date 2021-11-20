#include "SFML/Graphics.hpp"
#include <iostream>
#include <cmath>
#include "System.h"



int main() {
    System sys({10, 10});
    sys.AddParticles(10, 10, {1, 1});

    return 0;
}
