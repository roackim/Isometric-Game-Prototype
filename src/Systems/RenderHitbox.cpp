#include "RenderHitbox.h"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Components/Movement.hpp"
#include "SECS/ECS.hpp"
#include "Core/Isometric.h"


void ecs::system::renderHitboxWire(const Hitbox& hitbox, sf::RenderTarget& target, sf::RenderStates states)
{
    
    const sf::Vector3f& d = hitbox.dimensions;
    sf::Vector3f start = hitbox.position - sf::Vector3f(d.x, d.y, 0)/2.f;
    
    // create an array of 3 vertices that define a triangle primitive
    sf::VertexArray hitbox_vertex(sf::LineStrip, 11);
    // define the position of the triangle's points
    
    hitbox_vertex[0].position  = isometric::getProjection(start + sf::Vector3f(d.x, d.y,   0));
    hitbox_vertex[1].position  = isometric::getProjection(start + sf::Vector3f(  0, d.y,   0));
    hitbox_vertex[2].position  = isometric::getProjection(start + sf::Vector3f(  0, d.y, d.z));
    hitbox_vertex[3].position  = isometric::getProjection(start + sf::Vector3f(  0,   0, d.z));
    hitbox_vertex[4].position  = isometric::getProjection(start + sf::Vector3f(d.x,   0, d.z));
    hitbox_vertex[5].position  = isometric::getProjection(start + sf::Vector3f(d.x,   0,   0));
    hitbox_vertex[6].position  = isometric::getProjection(start + sf::Vector3f(d.x, d.y,   0));
    hitbox_vertex[7].position  = isometric::getProjection(start + sf::Vector3f(d.x, d.y, d.z));
    hitbox_vertex[8].position  = isometric::getProjection(start + sf::Vector3f(d.x,   0, d.z));
    hitbox_vertex[9].position  = isometric::getProjection(start + sf::Vector3f(d.x, d.y, d.z));
    hitbox_vertex[10].position = isometric::getProjection(start + sf::Vector3f(  0, d.y, d.z));

    const sf::Vector2f& p = hitbox_vertex[0].position;
    // // // define the color of the triangle's points
    sf::Color c = sf::Color(0xffedd0ff);
    hitbox_vertex[0].color  = c;
    hitbox_vertex[1].color  = c;
    hitbox_vertex[2].color  = c;
    hitbox_vertex[3].color  = c;
    hitbox_vertex[4].color  = c;
    hitbox_vertex[5].color  = c;
    hitbox_vertex[6].color  = c;
    hitbox_vertex[7].color  = c;
    hitbox_vertex[8].color  = c;
    hitbox_vertex[9].color  = c;
    hitbox_vertex[10].color = c;
    
    sf::VertexArray shadow(sf::LineStrip, 5);
    
    sf::Vector3f start2 = sf::Vector3f(start.x, start.y, 0);
    
    shadow[0].position  = isometric::getProjection(start2 + sf::Vector3f(d.x, d.y,   0));
    shadow[1].position  = isometric::getProjection(start2 + sf::Vector3f(  0, d.y,   0));
    shadow[2].position  = isometric::getProjection(start2 + sf::Vector3f(  0,   0,   0));
    shadow[3].position  = isometric::getProjection(start2 + sf::Vector3f(d.x,   0,   0));
    shadow[4].position  = isometric::getProjection(start2 + sf::Vector3f(d.x, d.y,   0));
    
    shadow[0].color  = c;
    shadow[1].color  = c;
    shadow[2].color  = c;
    shadow[3].color  = c;
    shadow[4].color  = c;
    
    target.draw(shadow, states);
    target.draw(hitbox_vertex, states);
    
}

void ecs::system::renderHitboxFull(const Hitbox& hitbox, sf::RenderTarget& target, sf::RenderStates states, bool black)
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
    
    quad[4].color = sf::Color(0xBBBBBBFF);
    quad[5].color = sf::Color(0xBBBBBBFF);
    quad[6].color = sf::Color(0xBBBBBBFF);
    quad[7].color = sf::Color(0xBBBBBBFF);
    
    quad[8].color  = sf::Color(0x999999FF);
    quad[9].color  = sf::Color(0x999999FF);
    quad[10].color = sf::Color(0x999999FF);
    quad[11].color = sf::Color(0x999999FF);
    

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
    
    if (black == true)
    {
        auto black_color = sf::Color(0x00000000);
        // 
        quad[0].color  = black_color;
        quad[1].color  = black_color;
        quad[2].color  = black_color;
        quad[3].color  = black_color;
        quad[4].color  = black_color;
        quad[5].color  = black_color;
        quad[6].color  = black_color;
        quad[7].color  = black_color;
        quad[8].color  = black_color;
        quad[9].color  = black_color;
        quad[10].color = black_color;
        quad[11].color = black_color;
    }
    
    if (not black) target.draw(shadow, states);
    target.draw(quad, sf::BlendNone);
}

void ecs::system::renderHitboxFullWithOutline(const Hitbox& hitbox, sf::RenderTarget& target, sf::RenderStates states)
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
    
    quad[4].color = sf::Color(0xffffffff);
    quad[5].color = sf::Color(0xffffffff);
    quad[6].color = sf::Color(0xffffffff);
    quad[7].color = sf::Color(0xffffffff);
    
    quad[8].color  = sf::Color(0x999999FF);
    quad[9].color  = sf::Color(0x999999FF);
    quad[10].color = sf::Color(0x999999FF);
    quad[11].color = sf::Color(0x999999FF);
    
    
    // create an array of 3 vertices that define a triangle primitive
    sf::VertexArray outline_vertex(sf::LineStrip, 11);
    // define the position of the triangle's points
    
    outline_vertex[0].position  = quad[0].position;  // isometric::getProjection(start + sf::Vector3f(d.x, d.y,   0));
    outline_vertex[1].position  = quad[1].position;  // isometric::getProjection(start + sf::Vector3f(  0, d.y,   0));
    outline_vertex[2].position  = quad[2].position;  // isometric::getProjection(start + sf::Vector3f(  0, d.y, d.z));
    outline_vertex[3].position  = quad[6].position;  // isometric::getProjection(start + sf::Vector3f(  0,   0, d.z));
    outline_vertex[4].position  = quad[7].position;  // isometric::getProjection(start + sf::Vector3f(d.x,   0, d.z));
    outline_vertex[5].position  = quad[11].position; // isometric::getProjection(start + sf::Vector3f(d.x,   0,   0));
    outline_vertex[6].position  = quad[8].position;  // isometric::getProjection(start + sf::Vector3f(d.x, d.y,   0));
    outline_vertex[7].position  = quad[9].position;  // isometric::getProjection(start + sf::Vector3f(d.x, d.y, d.z));
    outline_vertex[8].position  = quad[10].position; // isometric::getProjection(start + sf::Vector3f(d.x,   0, d.z));
    outline_vertex[9].position  = quad[9].position;  // isometric::getProjection(start + sf::Vector3f(d.x, d.y, d.z));
    outline_vertex[10].position = quad[2].position;  // isometric::getProjection(start + sf::Vector3f(  0, d.y, d.z));
    
    // // // define the color of the triangle's points
    sf::Color c = sf::Color(0x000000ff);
    outline_vertex[0].color  = c;
    outline_vertex[1].color  = c;
    outline_vertex[2].color  = c;
    outline_vertex[3].color  = c;
    outline_vertex[4].color  = c;
    outline_vertex[5].color  = c;
    outline_vertex[6].color  = c;
    outline_vertex[7].color  = c;
    outline_vertex[8].color  = c;
    outline_vertex[9].color  = c;
    outline_vertex[10].color = c;
    
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

    target.draw(shadow, states);
    target.draw(quad, sf::BlendNone);
    target.draw(outline_vertex, sf::BlendNone);
    
}