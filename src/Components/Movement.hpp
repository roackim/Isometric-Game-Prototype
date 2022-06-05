#pragma once

#include <SFML/System.hpp>


struct Movement
{
    sf::Vector3f velocity;      // current speed in meters/s
    sf::Vector3f acceleration;  // current acceleration
    sf::Vector3f delta;         // current step (velocity * dt)
    
    Movement() {}
    Movement(sf::Vector3f v_, sf::Vector3f a_=sf::Vector3f(0,0,0))
    : velocity(v_)
    , acceleration(a_) {}
};

struct Hitbox
{
    sf::Vector3f dimensions;
    sf::Vector3f position;
    
    Hitbox(sf::Vector3f d_, sf::Vector3f p_)
    : dimensions(d_), position(p_) {}
};