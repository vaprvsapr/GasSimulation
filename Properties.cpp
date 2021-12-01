//
// Created by nikita on 01.12.2021.
//

#include "Properties.h"

bool operator==(const Properties lhs, const Properties rhs)
{
    return lhs.radius == rhs.radius and lhs.mass == rhs.mass;
}
