#include "Renderer.h"

#include <SFML/Graphics.hpp>
#include "SECS/ECS.hpp"

#include "Core/Math.h"
#include "Components/Movement.hpp"

#include "Systems/RenderHitbox.h"
#include "Core/Isometric.h"


void ecs::system::renderer::renderEntities(sf::RenderTarget& target, sf::RenderStates states)
{
    for (auto e : renderer::entities)
    {

        ecs::system::renderHitboxFull(ecs::component::get<Hitbox>(e), target, states);
        // ecs::system::renderHitboxWire(ecs::component::get<Hitbox>(e), target, states);   
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

// start: top left corner
// end: bottom right corner
static void computeHitboxProjectionBounds(const Hitbox& a, sf::Vector2f& start, sf::Vector2f& end)
{
    //                          start +  at
    // al: a's leftmost point          _-''-_
    // ar: a's rightmost point        |-_  _-|
    // at: a's top point              |  `|  |
    // ab: a's bottom point        al '-_ |_-' ar
    //                                   ab  + end
    
    // compute outmost points
    auto al = sf::Vector3f(a.position.x - a.dimensions.x/2, a.position.y + a.dimensions.y/2, 0.f);
    auto ar = sf::Vector3f(a.position.x + a.dimensions.x/2, a.position.y - a.dimensions.y/2, 0.f);
    auto at = sf::Vector3f(a.position.x - a.dimensions.x/2, a.position.y - a.dimensions.y/2, a.position.z + a.dimensions.z); 
    auto ab = sf::Vector3f(a.position.x + a.dimensions.x/2, a.position.y + a.dimensions.y/2, 0.f);
    
    // px: Isometric projection in pixels on x axis
    // py: Isometric projection in pixels on y axis
    
    // compute their projections
    float alpx = isometric::getProjection(al).x;
    float arpx = isometric::getProjection(ar).x;
    
    float atpy = isometric::getProjection(at).y;
    float abpy = isometric::getProjection(ab).y;
    
    start = sf::Vector2f(alpx, atpy);
    end = sf::Vector2f(arpx, abpy);
    
    start.x = alpx;
}


static bool testOverlap(sf::Vector2f a_start, sf::Vector2f a_end, sf::Vector2f b_start, sf::Vector2f b_end)
{
    if ((b_start.x < a_end.x) and
        (b_start.y < a_end.y) and
        (b_end.x > a_start.x) and
        (b_end.y > a_start.y))
        return true;
    return false;
}


//  0: A and B are appart
// returns [0, 1, 2]
//  1: A is Behind B 
//  2: A is in front of B
static int computeRelativePosition(const Hitbox& a, const Hitbox& b)
{   
    sf::Vector2f a_start = {0,0};
    sf::Vector2f a_end   = {0,0};
    sf::Vector2f b_start = {0,0};
    sf::Vector2f b_end   = {0,0};
    
    computeHitboxProjectionBounds(a, a_start, a_end);
    computeHitboxProjectionBounds(b, b_start, b_end);
    
    if (testOverlap(a_start, a_end, b_start, b_end))
    {
        // necessitate more computation
        return 1;
    }
    
    return 0;
}


void ecs::system::renderer::sortRenderable()
{
    entities = ecs::entity::filter<Hitbox>();
    
    std::cout << " ----[" << entities.size() << "]---- " << std::endl;
    
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
            
            std::cout << entities[smallestindex] << " against " << entities[j] << ": " << std::endl;
            
            int res = computeRelativePosition(a, b);
            if (res == 2)
            {
                smallest = test;
                smallestindex = j;
                std::cout << "in front" << std::endl;
            }
            else if (res == 1 ) std::cout << "behind" << std::endl;
            else if (res == 0 ) std::cout << "none" << std::endl;
        }
        
        if (smallestindex != i) [[unlikely]]
        {
            uint tmp = entities[i];
            entities[i] = smallest;
            entities[smallestindex] = tmp;
        }
    }
}

// TODO implement spacial groups
// static void ecs::system::createEntityGroups()
// {
//     std::vector<uint> entities = ecs::entity::filter<Hitbox>();
    
//     auto& map = renderer::entity_to_group_map;
    
//     for (uint e1 : entities)
//     {
//         for (uint e2 : entities)
//         {
//             auto& h1 = ecs::component::get<Hitbox>(e1);
//             auto& h2 = ecs::component::get<Hitbox>(e2);
            
//             if (computeRelativePosition(h1, h2) != 0) // entities must be sorted in same group
//             {
//                 auto itr = map.find(e2);
//                 if ( itr != map.end()) // entity 2 is already part of a group
//                 {
//                     ; // TODO
//                 }
//             }
//         }
//     }
//     // renderer::entity_groups.push_back(entities);
// }