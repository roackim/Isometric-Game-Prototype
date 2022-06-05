#pragma once

#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>


#include "Components/Movement.hpp"

namespace ecs::system::renderer
{
    static std::vector<uint> entities;        
    
    void renderEntities(sf::RenderTarget& target, sf::RenderStates states);
    void sortRenderable();
    
}