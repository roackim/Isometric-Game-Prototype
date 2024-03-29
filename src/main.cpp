#include <SFML/Graphics.hpp>

#include <iostream>
#include <bitset>
#include <cstdint>
#include <string>
#include <stack>

#include "SECS/ECS.hpp"
#include "Components/Movement.hpp"
#include "Components/Controller.hpp"

#include "Systems/RenderHitbox.h"
#include "Systems/Controller.h"
#include "Systems/Movement.h"
#include "Systems/Collision.h"
#include "Systems/Renderer.h"

const uint SCALE = 3;
const uint WIN_WIDTH = 800;
const uint WIN_HEIGHT = 600;

int main(int argc, char* argv[])
{

    // create the window
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Physic Prototype");
    window.setFramerateLimit(140);

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

    uint e1 = ecs::entity::create();
    uint e2 = ecs::entity::create();
    uint e3 = ecs::entity::create();
    uint e4 = ecs::entity::create();
    uint e5 = ecs::entity::create();
    uint e6 = ecs::entity::create();
    uint e7 = ecs::entity::create();
    uint e8 = ecs::entity::create();

    ecs::component::add(Hitbox(sf::Vector3f(1, 1, 1), sf::Vector3f(12, 4, 2)), e1);
    ecs::component::add(Hitbox(sf::Vector3f(1, 1, 1.5), sf::Vector3f(16, 2, 0)), e2);
    ecs::component::add(Hitbox(sf::Vector3f(1, 3, 0.6), sf::Vector3f(18, 2, 0)), e4);
    
    ecs::component::add(Hitbox(sf::Vector3f(1.5, 1.5, 4), sf::Vector3f(12, -2, 0)), e3);
    
    ecs::component::add(Hitbox(sf::Vector3f(2, 2, 3), sf::Vector3f(12, 2, 0)), e5);
    
    ecs::component::add(Hitbox(sf::Vector3f(12, 12, 1), sf::Vector3f(13, 2, -1)), e8);
    ecs::component::get<Hitbox>(e1).gravity = true; // testing
    
    
    std::cout << e1 << ", " << e3 << std::endl;

    // ecs::component::add<Movement>(e3);
    // ecs::component::add<Controller>(e3);

    ecs::component::add<Movement>(e1);
    ecs::component::add<Controller>(e1);

    // ecs::component::add<Controller>(e2);
    // ecs::component::add<Movement>(e2);
    // ecs::component::get<Hitbox>(e2).gravity = true; // testing
    


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

            static sf::Clock swap_timer;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
            {

                if (swap_timer.getElapsedTime() > sf::milliseconds(140))
                {
                    if (ecs::entity::has<Controller>(e2))
                    {
                        std::cout << "removing controller component to #" << e2 << std::endl;
                        ecs::component::remove<Controller>(e2);
                        ecs::component::get<Movement>(e2).velocity = sf::Vector3f();
                    }
                    else
                    {
                        std::cout << "adding   controller component to #" << e2 << std::endl;
                        ecs::component::add<Controller>(e2);
                    }
                    swap_timer.restart();
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            {
                float growth = 0.1f;
                // ecs::component::get<Hitbox>(e1).dimensions += sf::Vector3f(growth, growth, growth);
                window.setFramerateLimit(100);
                
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
            {
                float growth = 0.1f;
                // ecs::component::get<Hitbox>(e1).dimensions += sf::Vector3f(-growth, -growth, -growth);
                window.setFramerateLimit(10);
                
            }


        }
        
        ecs::system::moveWithWASD(event);

        ecs::system::computeVelocities(dt);
        ecs::system::applyCollisions();
        ecs::system::moveEntitites();

        // drawing
        window.clear(sf::Color::Black);
        render_texture.clear(sf::Color::Black);
        render_texture.display();

        ecs::system::renderer::sortRenderable();
        ecs::system::renderer::renderEntities(render_texture, sf::RenderStates());
        

        // ecs::system::renderHitboxes(render_texture, sf::RenderStates());


        render_sprite.setTexture(render_texture.getTexture());
        render_sprite.setScale(SCALE, SCALE);
        window.draw(render_sprite);

        // end the current frame
        window.display();

    }

    return 0;

}