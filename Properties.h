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

bool operator==(Properties lhs, Properties rhs);

#endif //GASSIMULATION_PROPERTIES_H
