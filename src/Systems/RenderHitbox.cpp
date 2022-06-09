#include "RenderHitbox.h"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Components/Movement.hpp"
#include "SECS/ECS.hpp"
#include "Core/Isometric.h"


void ecs::system::renderHitboxes(sf::RenderTarget& target, sf::RenderStates states)
{
    auto entities = ecs::entity::filter<Hitbox>();

    for (auto& eid : entities)
    {
        // renderHitboxWire(ecs::component::get<Hitbox>(eid), target, states);
        renderHitboxFull(ecs::component::get<Hitbox>(eid), target, states);
    }
}

void ecs::system::renderHitboxWire(const Hitbox& hitbox, sf::RenderTarget& target, sf::RenderStates states)
{
    
    const sf::Vector3f& d = hitbox.dimensions;
    sf::Vector3f start = hitbox.position - sf::Vector3f(d.x, d.y, 0)/2.f;
    
    // create an array of 3 vertices that define a triangle primitive
    sf::VertexArray hitboxVertex(sf::LineStrip, 11);
    // define the position of the triangle's points
    
    hitboxVertex[0].position  = isometric::getProjection(start + sf::Vector3f(d.x, d.y,   0));
    hitboxVertex[1].position  = isometric::getProjection(start + sf::Vector3f(  0, d.y,   0));
    hitboxVertex[2].position  = isometric::getProjection(start + sf::Vector3f(  0, d.y, d.z));
    hitboxVertex[3].position  = isometric::getProjection(start + sf::Vector3f(  0,   0, d.z));
    hitboxVertex[4].position  = isometric::getProjection(start + sf::Vector3f(d.x,   0, d.z));
    hitboxVertex[5].position  = isometric::getProjection(start + sf::Vector3f(d.x,   0,   0));
    hitboxVertex[6].position  = isometric::getProjection(start + sf::Vector3f(d.x, d.y,   0));
    hitboxVertex[7].position  = isometric::getProjection(start + sf::Vector3f(d.x, d.y, d.z));
    hitboxVertex[8].position  = isometric::getProjection(start + sf::Vector3f(d.x,   0, d.z));
    hitboxVertex[9].position  = isometric::getProjection(start + sf::Vector3f(d.x, d.y, d.z));
    hitboxVertex[10].position = isometric::getProjection(start + sf::Vector3f(  0, d.y, d.z));

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
    
    shadow[0].position  = isometric::getProjection(start2 + sf::Vector3f(d.x, d.y,   0));
    shadow[1].position  = isometric::getProjection(start2 + sf::Vector3f(  0, d.y,   0));
    shadow[2].position  = isometric::getProjection(start2 + sf::Vector3f(  0,   0,   0));
    shadow[3].position  = isometric::getProjection(start2 + sf::Vector3f(d.x,   0,   0));
    shadow[4].position  = isometric::getProjection(start2 + sf::Vector3f(d.x, d.y,   0));
    
    // shadow[0].color  = sf::Color::Red;
    // shadow[1].color  = sf::Color::White;
    // shadow[2].color  = sf::Color::Blue;
    // shadow[3].color  = sf::Color::White;
    // shadow[4].color  = sf::Color::Cyan;
    
    target.draw(shadow, states);
    target.draw(hitboxVertex, states);
    
}

void ecs::system::renderHitboxFull(const Hitbox& hitbox, sf::RenderTarget& target, sf::RenderStates states)
{
    const sf::Vector3f& d = hitbox.dimensions;
    sf::Vector3f start = hitbox.position - sf::Vector3f(d.x, d.y, 0)/2.f;
    
    // create a quad
    sf::VertexArray quad(sf::Quads, 12);
    
    quad[0].position  = isometric::getProjection(start + sf::Vector3f(d.x, d.y,   0));
    quad[1].position  = isometric::getProjection(start + sf::Vector3f(  0, d.y,   0));
    quad[2].position  = isometric::getProjection(start + sf::Vector3f(  0, d.y, d.z));
    quad[3].position  = isometric::getProjection(start + sf::Vector3f(d.x, d.y, d.z));
    
    quad[4].position  = isometric::getProjection(start + sf::Vector3f(d.x, d.y, d.z));
    quad[5].position  = isometric::getProjection(start + sf::Vector3f(  0, d.y, d.z));
    quad[6].position  = isometric::getProjection(start + sf::Vector3f(  0,   0, d.z));
    quad[7].position  = isometric::getProjection(start + sf::Vector3f(d.x,   0, d.z));
    
    quad[8].position   = isometric::getProjection(start + sf::Vector3f(d.x, d.y,   0));
    quad[9].position   = isometric::getProjection(start + sf::Vector3f(d.x, d.y, d.z));
    quad[10].position  = isometric::getProjection(start + sf::Vector3f(d.x,   0, d.z));
    quad[11].position  = isometric::getProjection(start + sf::Vector3f(d.x,   0,   0));
    
    quad[0].color = sf::Color(0x222222FF);
    quad[1].color = sf::Color(0x222222FF);
    quad[2].color = sf::Color(0x222222FF);
    quad[3].color = sf::Color(0x222222FF);
    
    quad[4].color = sf::Color(0xFFFFFFFF);
    quad[5].color = sf::Color(0xFFFFFFFF);
    quad[6].color = sf::Color(0xFFFFFFFF);
    quad[7].color = sf::Color(0xFFFFFFFF);
    
    quad[8].color  = sf::Color(0x999999ff);
    quad[9].color  = sf::Color(0x999999ff);
    quad[10].color = sf::Color(0x999999ff);
    quad[11].color = sf::Color(0x999999ff);
    

    sf::VertexArray shadow(sf::Quads, 4);
    sf::Vector3f start2 = sf::Vector3f(start.x, start.y, 0);
    
    shadow[0].position  = isometric::getProjection(start2 + sf::Vector3f(d.x, d.y,   0));
    shadow[1].position  = isometric::getProjection(start2 + sf::Vector3f(  0, d.y,   0));
    shadow[2].position  = isometric::getProjection(start2 + sf::Vector3f(  0,   0,   0));
    shadow[3].position  = isometric::getProjection(start2 + sf::Vector3f(d.x,   0,   0));
    
    shadow[0].color = sf::Color(0x222222FF);
    shadow[1].color = sf::Color(0x222222FF);
    shadow[2].color = sf::Color(0x222222FF);
    shadow[3].color = sf::Color(0x222222FF);
    
    // sf::VertexArray shadow(sf::LineStrip, 5);
    // sf::Vector3f start2 = sf::Vector3f(start.x, start.y, 0);
    
    // shadow[0].position  = isometric::cartesianToIsomectric(start2 + sf::Vector3f(d.x, d.y,   0));
    // shadow[1].position  = isometric::cartesianToIsomectric(start2 + sf::Vector3f(  0, d.y,   0));
    // shadow[2].position  = isometric::cartesianToIsomectric(start2 + sf::Vector3f(  0,   0,   0));
    // shadow[3].position  = isometric::cartesianToIsomectric(start2 + sf::Vector3f(d.x,   0,   0));
    // shadow[4].position  = isometric::cartesianToIsomectric(start2 + sf::Vector3f(d.x, d.y,   0));
    
    // shadow[0].color  = sf::Color::Red;
    // shadow[1].color  = sf::Color::White;
    // shadow[2].color  = sf::Color::Blue;
    // shadow[3].color  = sf::Color::White;
    // shadow[4].color  = sf::Color::Cyan;
    
    if (hitbox.mode == true)
    {
        auto color = sf::Color(0x00000000);
        
        quad[0].color  = color;
        quad[1].color  = color;
        quad[2].color  = color;
        quad[3].color  = color;
        quad[4].color  = color;
        quad[5].color  = color;
        quad[6].color  = color;
        quad[7].color  = color;
        quad[8].color  = color;
        quad[9].color  = color;
        quad[10].color = color;
        quad[11].color = color;
    }
    
    target.draw(shadow, states);
    target.draw(quad, sf::BlendNone);
}