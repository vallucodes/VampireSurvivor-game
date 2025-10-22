#pragma once

#include <math.h>
#include <SFML/System.hpp>

inline float VecLength(sf::Vector2f a)
{
    return sqrtf(a.x*a.x + a.y*a.y);
}


inline sf::Vector2f VecNormalized(sf::Vector2f a)
{
    float length = VecLength(a);

    if (length != 0.0f)
        return sf::Vector2f(a.x / length, a.y / length);

    return a;
}
