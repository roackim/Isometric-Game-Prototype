#include <SFML/Graphics.hpp>

#include <iostream>
#include <bitset>
#include <cstdint>
#include <string>
#include <stack>

#include "SECS/ECS.hpp"
#include "Components/Core/Movement.hpp"
#include "Systems/Physic.h"

const uint SCALE = 3;
const uint WIN_WIDTH = 800;
const uint WIN_HEIGHT = 600;


void RenderHitboxes(ECS& ecs, sf::RenderTarget& target, sf::RenderStates states)
{
    auto entities = ecs.filterEntities<Hitbox>();

    for (auto& eid : entities)
    {
        RenderHitbox(ecs.getComponent<Hitbox>(eid), target, states);
    }
}

int main(int argc, char* argv[])
{

    // create the window
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Physic Prototype");
    window.setFramerateLimit(144);

    // used to render at low res
    sf::Sprite render_sprite;
    sf::RenderTexture render_texture;
    if (!render_texture.create(WIN_WIDTH / SCALE, WIN_HEIGHT / SCALE))
    {
        std::cout << "Could not instanciate render texture" << std::endl;
        return -1;
    }

    // physics based movement
    sf::Clock clock;
    sf::Time dt = clock.restart();


    ECS ecs;
    uint e1 = ecs.newEntity();
    uint e2 = ecs.newEntity();
    uint e3 = ecs.newEntity();

    ecs.addComponent(Hitbox(sf::Vector3f(1, 1, 1), sf::Vector3f(5, 2, 2)), e1);
    ecs.addComponent(Hitbox(sf::Vector3f(1, 1, 1), sf::Vector3f(7, 2, 1)), e2);
    ecs.addComponent(Hitbox(sf::Vector3f(2, 2, 2), sf::Vector3f(15, 4, 3)), e3);
    
    
    
    // auto var = ecs.getComponent<Hitbox>(e1);
    // var.dimensions.x = 2;
    // auto var2 = ecs.getComponent<Hitbox>(e1);
    // var2.dimensions.x = 3;
    // auto var3 = ecs.getComponent<Hitbox>(e1);
    // var3.dimensions.x = 3;
    

    while (window.isOpen())
    {
        dt = clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                    return(0);
                }
            }
        }

        // drawing
        window.clear(sf::Color::Black);
        render_texture.clear(sf::Color::Black);
        render_texture.display();
        
        RenderHitboxes(ecs, render_texture, sf::RenderStates());
        

        render_sprite.setTexture(render_texture.getTexture());
        render_sprite.setScale(SCALE, SCALE);
        window.draw(render_sprite);
        
        // end the current frame
        window.display();
    }

    return 0;

}