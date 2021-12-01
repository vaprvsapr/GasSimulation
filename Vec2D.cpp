//
// Created by nikita on 01.12.2021.
//

#include "Vec2D.h"

double Vec2D::SquareLengthVec2D() const
{
    return x * x + y * y;
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

Vec2D operator+(const Vec2D& lhs, const Vec2D& rhs)
{
    return {rhs.x + lhs.x, rhs.y + lhs.y};
}

Vec2D operator-(const Vec2D& lhs, const Vec2D& rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

bool operator==(const Vec2D& lhs, const Vec2D& rhs)
{
    return lhs.x == rhs.x and lhs.y == rhs.y;
}

Vec2D operator*(Vec2D vec, double& alpha)
{
    return {vec.x * alpha, vec.y * alpha};
}

Vec2D operator/(Vec2D vec, double alpha)
{
    return {vec.x / alpha, vec.y / alpha};
}
