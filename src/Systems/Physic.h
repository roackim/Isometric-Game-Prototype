#pragma once

#include <SFML/Graphics.hpp>

#include "Components/Core/Movement.hpp"


namespace iso
{
    // const sf::Vector2f vux( 18,   9);
    // const sf::Vector2f vuy(-18,   9);
    // const sf::Vector2f vuz(  0, -21);
    
    const sf::Vector2f vux( 12,   6);
    const sf::Vector2f vuy(-12,   6);
    const sf::Vector2f vuz(  0, -14);
    
    sf::Vector2f cartesianToIsomectric(sf::Vector3f v);
    
}

void RenderHitbox(const Hitbox& hitbox, sf::RenderTarget& target, sf::RenderStates states);