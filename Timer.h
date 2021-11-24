//
// Created by nikita on 26.11.2021.
//

#ifndef MAIN_CPP_TIMER_H
#define MAIN_CPP_TIMER_H

#include <ctime>
#include <string>
#include <iostream>

class Timer
{
private:
clock_t time;
std::string message;
public:
    Timer(std::string _message)
    {
        message = _message;
        time = clock();
    }
    ~Timer()
    {
        std::cout << clock() - time << message << std::endl;
    }
};


#endif //MAIN_CPP_TIMER_H
