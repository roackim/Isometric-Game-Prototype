#pragma once

#include <SFML/Window.hpp>
#include "SECS/ECS.hpp"


namespace ecs::system
{
    void moveWithWASD(sf::Event& event);
}