#include "Controller.h"
#include "SECS/ECS.hpp"

#include <iostream>

#include "Components/Movement.hpp"
#include "Components/Controller.hpp"

#include "Core/Math.h"

// reduction on the horizontal x axis (projected)
// to compensate for isomectric projection (2x : 1y) ratio
// static float perspectiveReduction(v2f dir)
// {
    
// }

void ecs::system::moveWithWASD(sf::Event& event)
{
    // ecs::component::get<HitBox(entity_id);
    float speed = 5.f;
    auto v = ecs::entity::filter<Movement, Controller>();
    
    for (uint e : v)
    {
        v3f vel;
        v2f dir;
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))    vel += sf::Vector3f(-1, -1,  0);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))    vel += sf::Vector3f(-1,  1,  0);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))    vel += sf::Vector3f( 1,  1,  0);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))    vel += sf::Vector3f( 1, -1,  0);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))   vel += sf::Vector3f( 0,  0,  1);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) vel += sf::Vector3f( 0,  0, -1);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))    dir += v2f( 0, -1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))    dir += v2f(-1,  0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))    dir += v2f( 0,  1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))    dir += v2f( 1,  0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))   vel += sf::Vector3f( 0,  0,  1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) vel += sf::Vector3f( 0,  0, -1);
        
        dir = math::clamp(dir, 1);
        
        dir *= speed;
        
        vel += math::addZAxis(dir);
        
        auto& c = ecs::component::get<Movement>(e);
        c.velocity += vel;
        
        static sf::Clock jump_timer;
        
        if (jump_timer.getElapsedTime() > sf::milliseconds(500))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                static uint i = 0;
                std::cout << "jumping: " << i++ << std::endl;
                c.velocity.z += 15.f;   
                jump_timer.restart();
            }
        }
    }
}

