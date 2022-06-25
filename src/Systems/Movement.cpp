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
        
        std::cout << "  vel: " << mv.velocity.x     << "  \t\t" << mv.velocity.y    << "\t\t" << mv.velocity.z << " >>>> " << dt.asMilliseconds() << std::endl;
        
        float dtime = dt.asSeconds();
        
        mv.acceleration = {0,0,0};
        
        // TODO fix this, its messy and unclean
        if (ecs::entity::has<Hitbox>(e))
        {
            Hitbox& h = ecs::component::get<Hitbox>(e);
            if (not h.gravity) continue;   
            
            mv.acceleration.z = -30.f;
        }
        std::cout << "  vel : x: " << mv.velocity.x << ", y: " << mv.velocity.y << ", z: " << mv.velocity.z << std::endl;
        
        // mv.acceleration = -mv.velocity * std::pow(0.2f, dtime);
        mv.velocity += mv.acceleration * dtime;
        mv.delta = mv.velocity * dtime;
        
        // friction
        // mv.velocity.z += -(mv.velocity.z - mv.velocity.z * std::pow(0.1f, dtime));
        // mv.velocity.z += -(1 - std::pow(0.1f, dtime)) * mv.velocity.z;
        float power = dt.asSeconds();
        float friction = std::pow(0.1f, power);
        
        mv.velocity.z *= friction;
        mv.velocity.y = 0;
        mv.velocity.x = 0;
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