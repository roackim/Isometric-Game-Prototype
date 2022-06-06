#pragma once

#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>


#include "Components/Movement.hpp"

namespace ecs::system::renderer
{
    // static std::unordered_map<uint, uint> entity_to_group_map;
    // static std::vector<std::vector<uint>> entity_groups; 
    static std::vector<uint> entities;        
    
    void renderEntities(sf::RenderTarget& target, sf::RenderStates states);
    void sortRenderable();
    
}