#pragma once

#include <SFML/System.hpp>

namespace isometric
{
    // const sf::Vector2f vux( 18,   9);
    // const sf::Vector2f vuy(-18,   9);
    // const sf::Vector2f vuz(  0, -21);
    
    const sf::Vector2f vux( 12,   6);
    const sf::Vector2f vuy(-12,   6);
    const sf::Vector2f vuz(  0, -14);
    
    sf::Vector2f getProjection(sf::Vector3f v);
    
}