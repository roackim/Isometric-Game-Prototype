#include "Controller.h"
#include "SECS/ECS.hpp"

#include <iostream>

#include "Components/Movement.hpp"
#include "Components/Controller.hpp"

void ecs::system::moveWithWASD(sf::Event& event)
{
    // ecs::component::get<HitBox(entity_id);
    float speed = 5.f;
    auto v = ecs::entity::filter<Movement, Controller>();
    
    for (uint e : v)
    {
        sf::Vector3f vel;
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))    vel += sf::Vector3f(-1, -1,  0);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))    vel += sf::Vector3f(-1,  1,  0);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))    vel += sf::Vector3f( 1,  1,  0);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))    vel += sf::Vector3f( 1, -1,  0);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))   vel += sf::Vector3f( 0,  0,  1);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) vel += sf::Vector3f( 0,  0, -1);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))    vel += sf::Vector3f( 0, -1,  0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))    vel += sf::Vector3f(-1,  0,  0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))    vel += sf::Vector3f( 0,  1,  0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))    vel += sf::Vector3f( 1,  0,  0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))   vel += sf::Vector3f( 0,  0,  1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) vel += sf::Vector3f( 0,  0, -1);
        
        auto& c = ecs::component::get<Movement>(e);
        c.velocity += vel * speed;
        
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

