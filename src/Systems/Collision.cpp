#include "Collision.h"
#include "SECS/ECS.hpp"

#include <cmath>

#include "Components/Movement.hpp"
#include "Core/Math.h"


static float zeroIfSmaller(float a, float x)
{
    if (math::abs(a) < x) return 0.0f;
    return a;
};

static v3f zeroIfSmaller(v3f v, float x)
{
    if (math::abs(v.x) < x) v.x = 0.f;
    if (math::abs(v.y) < x) v.y = 0.f;
    if (math::abs(v.z) < x) v.z = 0.f;
    
    return v;
}

// A is tested against B, this assumes B is static
static bool sweptAABB(Hitbox& a, Movement& mv, const Hitbox& b, float& dist)
{
    sf::Vector3f& v = mv.delta;
    
    v = zeroIfSmaller(v, 0.00001f);
    
    float dxf, dyf, dzf; // difference, between nearest edges 
    float dxb, dyb, dzb; // difference, between furthest edges
    
    // TODO please fix this, this is horrible
    // shrinks the slightest possible the hitbox of the moving entity to avoid bound bug
    
    // std::cout << a.dimensions.x << std::endl;
    
    // a is considered on the left by default
    dxf = (b.position.x - b.dimensions.x/2) - (a.position.x + a.dimensions.x/2);
    dxb = (b.position.x + b.dimensions.x/2) - (a.position.x - a.dimensions.x/2);    
    if (v.x < 0.f) math::swap(dxf, dxb); // if a is on the right side, swap
    
    // a is considered on the smaller y by default
    dyf = (b.position.y - b.dimensions.y/2) - (a.position.y + a.dimensions.y/2);
    dyb = (b.position.y + b.dimensions.y/2) - (a.position.y - a.dimensions.y/2);  
    if (v.y < 0.f) math::swap(dyf, dyb); // if otherwise swap values
    
    // a is considered on bottom by default
    dzf = (b.position.z) - (a.position.z + a.dimensions.z);
    dzb = (b.position.z + b.dimensions.z) - (a.position.z); 
    if (v.z < 0.f) math::swap(dzf, dzb);
    
    // used to compute time of collision
    float dtxf, dtyf, dtzf; // time of collision for each axis 
    float dtxb, dtyb, dtzb; // time of leaving for each axis
    
    // compute time of collision per axis
    dtxf = dxf / v.x; // x
    dtxb = dxb / v.x;   
    
    dtyf = dyf / v.y; // y
    dtyb = dyb / v.y;
    
    dtzf = dzf / v.z; // z
    dtzb = dzb / v.z;
    
    
    // fix traversing bug
    dtxf = zeroIfSmaller(dtxf, 0.0001f);
    dtyf = zeroIfSmaller(dtyf, 0.0001f);
    dtzf = zeroIfSmaller(dtzf, 0.0001f);
    
    // determine where the constraint comes from
    float dtentry = math::max(dtxf, dtyf, dtzf);
    float dtexit  = math::min(dtxb, dtyb, dtzb);
    
    // apparently useless
    // float dx = math::closestToZero(dxf, dxb);
    // float dy = math::closestToZero(dyf, dyb);
    // float dz = math::closestToZero(dzf, dzb);
    
    std::cout << "dt: " << dtentry  << "\t\t" << dtexit << std::endl;
    // std::cout << "df: " << dxf      << "\t\t" << dyf    << "\t\t" << dzf << std::endl;
    // std::cout << "db: " << dxb      << "\t\t" << dyb    << "\t\t" << dzb << std::endl;
    std::cout << "delta: " << v.x     << "\t\t" << v.y    << "\t\t" << v.z << std::endl;
    // std::cout << "me: "  << dx      << "\t\t" << dy     << "\t\t" << dz << std::endl;
    std::cout << "dtf: " << dtxf    << "\t\t" << dtyf   << "\t\t" << dtzf << "\t" << std::endl;
    
    // debug
    static unsigned long long cpt = 0;
    
    // cases where there is no collision
    if ((dtentry < 0.f)
    or (dtentry >= dtexit) // allow for getting out of the insides of a box
    or ((dtxf < 0.f) and (dtyf < 0.f) and (dtzf < 0.f)) 
    or (dtxf > 1.f) // going to collide but not this frame 
    or (dtyf > 1.f)
    or (dtzf > 1.f)
    )
    {
        std::cout << "----- FALSE ----- " << ++cpt << std::endl;
        return false;
    }
    else // a collision happened
    {
        // collision response
        int i = math::maxindex(dtxf, dtyf, dtzf);
        
        // only restraint one axis
        if (i == 1) 
        {
            v.x *= (dtentry); // - 100*std::numeric_limits<float>::epsilon());// // - 0.0001 -> avoid staying in collision state after resolution
            mv.velocity.x = 0;
            std::cout << "New v.x " << v.x << std::endl;
        }
        else if (i == 2) 
        {
            v.y *= (dtentry); // - 100*std::numeric_limits<float>::epsilon());// - 0.001f);
            mv.velocity.y = 0;
            std::cout << "New v.y " << v.y << std::endl;
        }
        else if (i == 3) 
        {
            v.z *= (dtentry); // - 100*std::numeric_limits<float>::epsilon());// - 0.001f);
            mv.velocity.z = 0;
            std::cout << "New v.z " << v.z << std::endl;
        }
        
        std::cout << "[[[[[ TRUE  ]]]]] " << ++cpt << std::endl;
        return true;
    }
}


void ecs::system::applyCollisions() // TODO implement range checks, to avoid checking against everyone
{
    // ecs::component::get<HitBox(entity_id);
    float speed = 7.f;
    auto v = ecs::entity::filter<Movement, Hitbox>();
    auto v2 = ecs::entity::filter<Hitbox>();
    
    for (uint e1 : v)
    {
        if (ecs::component::get<Movement>(e1).velocity == sf::Vector3f(0,0,0)) // e1 is cappable of moving, but is static
        {
            continue; // skip iteration   
        }
        
        float dist = 0.f;
        Movement& mv = ecs::component::get<Movement>(e1);
        Hitbox& h1  = ecs::component::get<Hitbox>(e1);
        for (uint e2 : v2)
        {
            if (e1 == e2) continue; // avoid checking collisions against self
            
            auto& h2  = ecs::component::get<Hitbox>(e2);
            
            if (ecs::entity::has<Movement>(e2)) // both entities are moving
            {
                // TODO properly 
                bool res = sweptAABB(h1, mv, h2, dist);
            }
            else // second entity is static
            {
                // std::cout << ">>> " << e1 << " against " << e2 << " <<<" <<std::endl;
                sweptAABB(h1, mv, h2, dist);
            }   
        }
    }
}