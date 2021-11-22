//
// Created by nikita on 17.11.2021.
//

#ifndef GASSIMULATION_PROPERTIES_H
#define GASSIMULATION_PROPERTIES_H


class Properties {
public:
    double radius;
    double mass;

    Properties() = default;
};

bool operator==(const Properties lhs, const Properties rhs)
{
    return lhs.radius == rhs.radius and lhs.mass == rhs.mass;
}

#endif //GASSIMULATION_PROPERTIES_H
