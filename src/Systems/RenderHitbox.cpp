#include "RenderHitbox.h"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Components/Movement.hpp"
#include "SECS/ECS.hpp"

sf::Vector2f iso::cartesianToIsomectric(sf::Vector3f v)
{ 
    return v.x * iso::vux + v.y * iso::vuy + v.z * iso::vuz;
}


void ecs::system::renderHitboxes(sf::RenderTarget& target, sf::RenderStates states)
{
    auto entities = ecs::entity::filter<Hitbox>();

    for (auto& eid : entities)
    {
        renderHitbox(ecs::component::get<Hitbox>(eid), target, states);
    }
}

static void ecs::system::renderHitbox(const Hitbox& hitbox, sf::RenderTarget& target, sf::RenderStates states)
{
    
    const sf::Vector3f& d = hitbox.dimensions;
    sf::Vector3f start = hitbox.position - sf::Vector3f(d.x, d.y, 0)/2.f;
    
    // create an array of 3 vertices that define a triangle primitive
    sf::VertexArray hitboxVertex(sf::LineStrip, 11);
    // define the position of the triangle's points
    
    hitboxVertex[0].position  = iso::cartesianToIsomectric(start + sf::Vector3f(d.x, d.y,   0));
    hitboxVertex[1].position  = iso::cartesianToIsomectric(start + sf::Vector3f(  0, d.y,   0));
    hitboxVertex[2].position  = iso::cartesianToIsomectric(start + sf::Vector3f(  0, d.y, d.z));
    hitboxVertex[3].position  = iso::cartesianToIsomectric(start + sf::Vector3f(  0,   0, d.z));
    hitboxVertex[4].position  = iso::cartesianToIsomectric(start + sf::Vector3f(d.x,   0, d.z));
    hitboxVertex[5].position  = iso::cartesianToIsomectric(start + sf::Vector3f(d.x,   0,   0));
    hitboxVertex[6].position  = iso::cartesianToIsomectric(start + sf::Vector3f(d.x, d.y,   0));
    hitboxVertex[7].position  = iso::cartesianToIsomectric(start + sf::Vector3f(d.x, d.y, d.z));
    hitboxVertex[8].position  = iso::cartesianToIsomectric(start + sf::Vector3f(d.x,   0, d.z));
    hitboxVertex[9].position  = iso::cartesianToIsomectric(start + sf::Vector3f(d.x, d.y, d.z));
    hitboxVertex[10].position = iso::cartesianToIsomectric(start + sf::Vector3f(  0, d.y, d.z));

    const sf::Vector2f& p = hitboxVertex[0].position;
    // // // define the color of the triangle's points
    // hitboxVertex[0].color  = color;
    // hitboxVertex[1].color  = color;
    // hitboxVertex[2].color  = color;
    // hitboxVertex[3].color  = color;
    // hitboxVertex[4].color  = color;
    // hitboxVertex[5].color  = color;
    // hitboxVertex[6].color  = color;
    // hitboxVertex[7].color  = color;
    // hitboxVertex[8].color  = color;
    // hitboxVertex[9].color  = color;
    // hitboxVertex[10].color = color;
    
    sf::VertexArray shadow(sf::LineStrip, 5);
    
    sf::Vector3f start2 = sf::Vector3f(start.x, start.y, 0);
    
    shadow[0].position  = iso::cartesianToIsomectric(start2 + sf::Vector3f(d.x, d.y,   0));
    shadow[1].position  = iso::cartesianToIsomectric(start2 + sf::Vector3f(  0, d.y,   0));
    shadow[2].position  = iso::cartesianToIsomectric(start2 + sf::Vector3f(  0,   0,   0));
    shadow[3].position  = iso::cartesianToIsomectric(start2 + sf::Vector3f(d.x,   0,   0));
    shadow[4].position  = iso::cartesianToIsomectric(start2 + sf::Vector3f(d.x, d.y,   0));
    
    // shadow[0].color  = sf::Color::Red;
    // shadow[1].color  = sf::Color::White;
    // shadow[2].color  = sf::Color::Blue;
    // shadow[3].color  = sf::Color::White;
    // shadow[4].color  = sf::Color::Cyan;
    
    target.draw(shadow, states);
    target.draw(hitboxVertex, states);
    
}