cmake_minimum_required(VERSION 3.20)
project(GasSimulation)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML COMPONENTS graphics audio REQUIRED)

add_executable(GasSimulation main.cpp Vec2D.h Particle.h Properties.h System.h Timer.h Properties.cpp Vec2D.cpp Particle.cpp System.cpp)

target_link_libraries(GasSimulation sfml-graphics sfml-audio)