#include "Movement.h"

#include <SFML/System.hpp>
#include "SECS/ECS.hpp"

#include <Components/Movement.hpp>
#include <Systems/Collision.h>


void ecs::system::computeVelocities(sf::Time& dt)
{
    auto v = ecs::entity::filter<Movement>();
    
    for (uint e : v)
    {
        Movement& mv = ecs::component::get<Movement>(e);   
        mv.delta = mv.velocity * dt.asSeconds();
    }
}

void ecs::system::moveEntitites()
{
    auto v = ecs::entity::filter<Movement, Hitbox>();
    
    for (uint e : v)
    {
        Movement& mv = ecs::component::get<Movement>(e);   
        Hitbox& hb = ecs::component::get<Hitbox>(e);
        
        hb.position += mv.delta;
        
        // TODO AABB collision detection + response
    }
}