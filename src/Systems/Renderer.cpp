#include "Renderer.h"

#include <SFML/Graphics.hpp>
#include "SECS/ECS.hpp"

#include "Core/Math.h"
#include "Components/Movement.hpp"

#include "Systems/RenderHitbox.h"


static bool inFrontOf(const Hitbox& a, const Hitbox& b);

void ecs::system::renderer::renderEntities(sf::RenderTarget& target, sf::RenderStates states)
{
    for (auto e : entities)
    {
        ecs::system::renderHitboxFull(ecs::component::get<Hitbox>(e), target, states);
        // ecs::system::renderHitboxWire(ecs::component::get<Hitbox>(e), target, states);
        
    }
}

void ecs::system::renderer::sortRenderable()
{
    entities = ecs::entity::filter<Hitbox>();
    
    std::cout << " ----[" << entities.size() << "]---- " << std::endl;
    
    std::cout << "[";
    for (auto v : entities)
    {
        std::cout << v << " ";
    }
    std::cout << "]" << std::endl;
    
    for (uint i = 0; i < entities.size(); ++i)
    {
        
        
        uint smallest = entities[i];
        uint smallestindex = i;
            
        for (uint j = i+1; j < entities.size(); ++j)
        {
            uint test = entities[j];
            
            // Comparison
            const Hitbox a = ecs::component::get<Hitbox>(entities[smallestindex]);
            const Hitbox b = ecs::component::get<Hitbox>(entities[j]);
            
            std::cout << entities[smallestindex] << " against " << entities[j] << ": ";
            if (inFrontOf(a, b))
            {
                smallest = test;
                smallestindex = j;
                std::cout << "in front" << std::endl;
            }
            else std::cout << "behind" << std::endl;
        }
        
        if (smallestindex != i) [[unlikely]]
        {
            uint tmp = entities[i];
            entities[i] = smallest;
            entities[smallestindex] = tmp;
        }
        
        std::cout << "[";
        for (auto v : entities)
        {
            std::cout << v << " ";
        }
        std::cout << "]" << std::endl;
    }
}

// returns either [-1, 1]
static int convertRange(bool x)
{
    return (x * 2) - 1 ;   
}

// returns either [-2, 0, 2]
// -2: f1 and f2 are both negatives
//  0: f1 and f2 are of opposite sign
// +2: f1 and f2 are both positives
static int signSum(float f1, float f2)
{
    return convertRange(math::sign(f1)) + convertRange(math::sign(f2));
}

static bool almostZero(float f)
{
    bool res = (math::abs(f) < 0.00001f);
    return res;  
}

// returns false if A is in front of B
static bool inFrontOf(const Hitbox& a, const Hitbox& b)
{   
    
    float dxf, dyf, dzf; // difference, between nearest edges 
    float dxb, dyb, dzb; // difference, between furthest edges 
    
    dxf = (a.position.x - a.dimensions.x/2) - (b.position.x + b.dimensions.x/2);
    dxb = (a.position.x + a.dimensions.x/2) - (b.position.x - b.dimensions.x/2);  
    
    dyf = (a.position.y - a.dimensions.y/2) - (b.position.y + b.dimensions.y/2);
    dyb = (a.position.y + a.dimensions.y/2) - (b.position.y - b.dimensions.y/2);
    
    // fixes glitch of almost superposition
    if (dxf == 0.0f) dxf = dxb;
    if (dxb == 0.0f) dxb = dxf;
    if (dyf == 0.0f) dyf = dyb;
    if (dyb == 0.0f) dyb = dyf;
    
    
    int sx = signSum(dxf, dxb);
    int sy = signSum(dyf, dyb);
    
    std::cout << "\t" << sx << " " << sy << " \t ";
    
    if (math::abs(sx+sy) == 2) // can discriminate
    {
        if (sx ==  2) return true;
        if (sx == -2) return false;
        if (sy ==  2) return true;
        if (sy == -2) return false;
    }
    else if ((sx + sy) == 4)
    {
        return true;
    }
    else if ((sx + sy) == -4) // cannot discriminate with base
    {
        return false;
    }
    else if ((sx+sy) == 0)
    {
        float sdx = math::closestToZero(dxf, dxb);
        float sdy = math::closestToZero(dyf, dyb);
        
        // if (sx < 0) math::swap(sdx, mdx);
        // if (sy < 0) math::swap(sdy, mdy);
                
        std::cout << math::abs(sdx / sdy) << "\t" ;
        // std::cout << dxf << "\t" << dxb << "\t" << dyf << "\t" << dyb << " ";
        
        if (math::abs(sdx / sdy) > 1) return false;
        return true;
    }
    else if (sx == 0 and sy == 0) [[unlikely]]// both shape are superposed
    {
        // TODO
        return false;
    }
    return true;
}