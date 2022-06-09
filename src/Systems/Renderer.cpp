#include "Renderer.h"

#include <SFML/Graphics.hpp>
#include "SECS/ECS.hpp"

#include "Core/Math.h"
#include "Components/Movement.hpp"

#include "Systems/RenderHitbox.h"
#include "Core/Isometric.h"


void ecs::system::renderer::renderEntities(sf::RenderTarget& target, sf::RenderStates states)
{
    // for (unsigned i = v.size(); i-- > 0; )
    for (uint i = entities.size(); i-- > 0; )
    {

        ecs::system::renderHitboxFull(ecs::component::get<Hitbox>(entities[i]), target, states);
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
    return convertRange(math::signbool(f1)) + convertRange(math::signbool(f2));
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
    // al: a's leftmost point          _-'''-_
    // ar: a's rightmost point        |-_   _-|
    // at: a's top point              |  `|`  |
    // ab: a's bottom point        al '-_ | _-' ar
    //                                   ab  + end
    
    // compute outmost points
    auto al = sf::Vector3f(a.position.x - a.dimensions.x/2, a.position.y + a.dimensions.y/2, 0.f);
    auto ar = sf::Vector3f(a.position.x + a.dimensions.x/2, a.position.y - a.dimensions.y/2, 0.f);
    auto at = sf::Vector3f(a.position.x - a.dimensions.x/2, a.position.y - a.dimensions.y/2, a.position.z + a.dimensions.z); 
    auto ab = sf::Vector3f(a.position.x + a.dimensions.x/2, a.position.y + a.dimensions.y/2, a.position.z);
    
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
    
    if (not testOverlap(a_start, a_end, b_start, b_end))
    {
        return -1;   
    }
    
    // TODO add z axis differential
            
    float dxf, dyf, dzf; // difference, between nearest edges 
    float dxb, dyb, dzb; // difference, between furthest edges 
    
    dxf = (a.position.x - a.dimensions.x/2) - (b.position.x + b.dimensions.x/2);
    dxb = (a.position.x + a.dimensions.x/2) - (b.position.x - b.dimensions.x/2);  
    
    dyf = (a.position.y - a.dimensions.y/2) - (b.position.y + b.dimensions.y/2);
    dyb = (a.position.y + a.dimensions.y/2) - (b.position.y - b.dimensions.y/2);
    
    dzf = (a.position.z) - (b.position.z + b.dimensions.z);
    dzb = (a.position.z + a.dimensions.z) - (b.position.z);

    
    // fixes glitch of almost superposition
    if (dxf == 0.f) dxf = dxb;
    if (dxb == 0.f) dxb = dxf;
    if (dyf == 0.f) dyf = dyb;
    if (dyb == 0.f) dyb = dyf;
    if (dzf == 0.f) dzf = dzb;
    if (dzb == 0.f) dzb = dzf;
    
    int sx = signSum(dxf, dxb);
    int sy = signSum(dyf, dyb);
    int sz = signSum(dzf, dzb);
    
    if (sz == 2) return 2;
    else if (sz == -2) return 1;
    else if (math::abs(sx+sy) == 2) // can discriminate
    {
        if (sx ==  2) return 2;
        if (sx == -2) return 1;
        if (sy ==  2) return 2;
        if (sy == -2) return 1;
    }
    else if ((sx + sy) == 4)
    {
        return 2;
    }
    else if ((sx + sy) == -4) // cannot discriminate with base
    {
        return 1;
    }
    else if ((sx+sy) == 0)
    {
        float sdx = math::closestToZero(dxf, dxb);
        float sdy = math::closestToZero(dyf, dyb);
        
        if (math::abs(sdx / sdy) > 1) return 1;
        return 2;
    }
    else if (sx == 0 and sy == 0) [[unlikely]]// both shape are superposed
    {
        // TODO 
        return 1;
    }
    return 2;
}

void ecs::system::renderer::buildRenderTree()
{
    RenderTree render_tree; // empty render tree
    
    entities = ecs::entity::filter<Hitbox>();
    
    // create one node per entity
    for (uint e1 : entities)
    {
        render_tree.createNode(e1);
    }
    
    // build tree
    for (uint i = 0 ; i < entities.size(); ++i)
    {
        for (uint j = i+1 ; j < entities.size(); ++j)
        {
            uint e1 = entities[i];
            uint e2 = entities[j];
            
            if (e1 == e2) continue;
            
            const Hitbox a = ecs::component::get<Hitbox>(e1);
            const Hitbox b = ecs::component::get<Hitbox>(e2);
            
            int res = computeRelativePosition(a, b);
            
            // std::cout << e1 << " : " << e2 << " ";
            // 
            // if (res == 1) std::cout << "behind" << std::endl;
            // else if (res == 2) std::cout << "infront" << std::endl;
            // else std::cout << "none" << std::endl;
            
            if (res == 1) render_tree.addChildTo(e2, e1);
            else if (res == 2) render_tree.addChildTo(e1, e2);       
        }
    }
    
    entities = render_tree.topologicalSort();
        
    // for (uint i: entities) std::cout << i << ", ";   
    // std::cout << std::endl;
}   




void ecs::system::renderer::sortRenderable()
{
    buildRenderTree();
}