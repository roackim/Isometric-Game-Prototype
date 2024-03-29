#pragma once

#include <SFML/Graphics.hpp>

#include "Components/Movement.hpp"


namespace ecs::system
{
    void renderHitboxWire(const Hitbox& hitbox, sf::RenderTarget& target, sf::RenderStates states);
    void renderHitboxFull(const Hitbox& hitbox, sf::RenderTarget& target, sf::RenderStates states,  bool black = false);
    void renderHitboxFullWithOutline(const Hitbox& hitbox, sf::RenderTarget& target, sf::RenderStates states);
    
}