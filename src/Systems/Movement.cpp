#include "Movement.h"

#include "Core/Math.h"
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
        
        // TODO fix this, its messy and unclean
        if (ecs::entity::has<Hitbox>(e))
        {
            Hitbox& h = ecs::component::get<Hitbox>(e);
            if (not h.gravity) continue;   
            
            float gravity_acceleration = -4; // if entity has gravity add Z component to acceleration
            mv.velocity.z += gravity_acceleration;
        }
        
        float dtime = dt.asSeconds();
        mv.delta = mv.velocity * dtime;
        mv.velocity *= 0.5f;
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