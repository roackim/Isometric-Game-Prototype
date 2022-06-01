#include "moveEntities.h"

#include <SFML/System.hpp>
#include "SECS/ECS.hpp"

#include <Components/Movement.hpp>

void ecs::system::moveEntitites(sf::Time& dt)
{
    auto v = ecs::entity::filter<Movement, Hitbox>();
    
    for (uint e : v)
    {
        Movement& mv = ecs::component::get<Movement>(e);   
        Hitbox& hb = ecs::component::get<Hitbox>(e);
        
        hb.position += mv.velocity * dt.asSeconds();
        
        // TODO AABB collision detection + response
    }
}