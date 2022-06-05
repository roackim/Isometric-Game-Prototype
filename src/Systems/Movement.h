#pragma once 

#include <SFML/System.hpp>

namespace ecs::system
{
    void computeVelocities(sf::Time& dt); 
    void moveEntitites(); 
    
}
