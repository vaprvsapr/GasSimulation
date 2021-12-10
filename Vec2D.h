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

    [[nodiscard]] double SquareLengthVec2D() const;


};

double ScalarMultiplication(const Vec2D& lhs,const Vec2D& rhs);

Vec2D ProjectionVec2D(const Vec2D& lhs,const Vec2D& rhs);

Vec2D ConnectVec2D(const Vec2D& lhs, const Vec2D& rhs);

Vec2D operator+(const Vec2D& lhs, const Vec2D& rhs);

Vec2D operator-(const Vec2D& lhs, const Vec2D& rhs);

bool operator==(const Vec2D& lhs, const Vec2D& rhs);

Vec2D operator*(Vec2D vec, double& alpha);

Vec2D operator/(Vec2D vec, double alpha);

#endif //GASSIMULATION_VEC2D_H
