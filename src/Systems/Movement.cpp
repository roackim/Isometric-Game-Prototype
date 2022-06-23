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
        std::cout << "vel: " << mv.velocity.x << ", " << mv.velocity.y << ", " << mv.velocity.z << std::endl;
        
        if (ecs::entity::has<Hitbox>(e))
        {
            Hitbox& h = ecs::component::get<Hitbox>(e);
            if (not h.gravity) continue;   
            
            mv.velocity.z -= 65 * dt.asSeconds();
        }
        
        mv.delta = mv.velocity * dt.asSeconds();
        
        mv.velocity.x *= 0.25f * (1 - dt.asSeconds());
        mv.velocity.y *= 0.25f * (1 - dt.asSeconds());
        mv.velocity.z *= 0.95f * (1 - dt.asSeconds());
        
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