//
// Created by nikita on 17.11.2021.
//

#ifndef GASSIMULATION_VEC2D_H
#define GASSIMULATION_VEC2D_H

#include <cmath>

class Vec2D {
public:
    double x;
    double y;

    Vec2D() = default;

    Vec2D(double _x, double _y)
    {
        x = _x;
        y = _y;
    }

    [[nodiscard]] double Modulus() const
    {
        return sqrt(x * x + y * y);
    }

    [[nodiscard]] double SquareLengthVec2D() const
    {
        return x * x + y * y;
    }

};

Vec2D operator+(const Vec2D& lhs, const Vec2D& rhs)
{
    return {rhs.x + lhs.x, rhs.y + lhs.y};
}

Vec2D operator-(const Vec2D& lhs, const Vec2D& rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

double ScalarMultiplication(const Vec2D& lhs,const Vec2D& rhs)
{
    return rhs.x * lhs.x + rhs.y * lhs.y;
}

Vec2D ProjectionVec2D(const Vec2D& lhs,const Vec2D& rhs)
{
    double k = ScalarMultiplication(lhs, rhs) / rhs.SquareLengthVec2D();
    return {rhs.x * k, rhs.y * k};
}

Vec2D ConnectVec2D(const Vec2D& lhs, const Vec2D& rhs)
{
    return {rhs.x - lhs.x, rhs.y - lhs.y};
}



#endif //GASSIMULATION_VEC2D_H
