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

static bool almostZero(float f)
{
    // std::cout << f << " < " << "0.00001f = " << (math::abs(f) < 0.00001f) << std::endl;
    if (math::abs(f) < 0.00001f) return true;
    return false;   
}

// A is tested against B, this assumes B is static
static bool sweptAABB(Hitbox& a, Movement& mv, const Hitbox& b, float& dist)
{
    sf::Vector3f& v = mv.delta;
    
    // std::cout << "  step: x: " << v.x << ", y: " << v.y << ", z: " << v.z << std::endl;

    v = zeroIfSmaller(v, 0.00001f);
    
    float ax1 = a.position.x - a.dimensions.x / 2;
    float ax2 = a.position.x + a.dimensions.x / 2;
    float ay1 = a.position.y - a.dimensions.y / 2;
    float ay2 = a.position.y + a.dimensions.y / 2;
    float az1 = a.position.z;
    float az2 = a.position.z + a.dimensions.z;
    
    float bx1 = b.position.x - b.dimensions.x / 2;
    float bx2 = b.position.x + b.dimensions.x / 2;
    float by1 = b.position.y - b.dimensions.y / 2;
    float by2 = b.position.y + b.dimensions.y / 2;
    float bz1 = b.position.z;
    float bz2 = b.position.z + b.dimensions.z;
    
    float dxf, dyf, dzf; // difference, between nearest edges 
    float dxb, dyb, dzb; // difference, between furthest edges
    
    // a is considered on the left by default
    dxf = bx1 - ax2;
    dxb = bx2 - ax1;    
    if (v.x < 0.f) math::swap(dxf, dxb); // if a is on the right side, swap
    
    // a is considered on the smaller y by default
    dyf = by1 - ay2;
    dyb = by2 - ay1;  
    if (v.y < 0.f) math::swap(dyf, dyb); // if otherwise swap values
    
    // a is considered on bottom by default
    dzf = bz1 - az2;
    dzb = bz2 - az1; 
    if (v.z < 0.f) math::swap(dzf, dzb);
    
    // compute estimated time before collision per axis
    float dtxf = dxf / v.x; // x
    float dtxb = dxb / v.x;   
    
    float dtyf = dyf / v.y; // y
    float dtyb = dyb / v.y;
    
    float dtzf = dzf / v.z; // z
    float dtzb = dzb / v.z;
    
    // fix traversing bug
    dtxf = zeroIfSmaller(dtxf, 0.0001f);
    dtyf = zeroIfSmaller(dtyf, 0.0001f);
    dtzf = zeroIfSmaller(dtzf, 0.0001f);
    
    // edge fix
    uint sum = almostZero(v.x) + almostZero(v.y) + almostZero(v.z);
    
    bool xout = ax1 >= bx2 or ax2 <= bx1;
    bool yout = ay1 >= by2 or ay2 <= by1;
    bool zout = az1 >= bz2 or az2 <= bz1;
    
    // std::cout << "  out: " << xout << " " << yout << " " << zout << std::endl;
    // std::cout << "  zro: " << almostZero(v.x) << " " << almostZero(v.y) << " " << almostZero(v.z) << std::endl;
    
    // determine where the constraint comes from
    float dtentry = math::max(dtxf, dtyf, dtzf);
    float dtexit  = math::min(dtxb, dtyb, dtzb);
    
    
    // std::cout << "  dt: " << dtentry    << "  \t\t" << dtexit << std::endl;
    // std::cout << "  df: " << dxf        << "  \t\t" << dyf    << "\t\t" << dzf << std::endl;
    // std::cout << "  db: " << dxb        << "  \t\t" << dyb    << "\t\t" << dzb << std::endl;
    // std::cout << "  delta: " << v.x     << "  \t\t" << v.y    << "\t\t" << v.z << std::endl;
    // std::cout << "me: "  << dx       << "  \t\t" << dy     << "\t\t" << dz << std::endl;
    // std::cout << "  dtf: " << dtxf      << "  \t\t" << dtyf   << "\t\t" << dtzf << "\t" << std::endl;
    
    // cases where there is no collision
    if ((dtentry < 0.f)
    or (dtxf > 1.f) // going to collide but not this frame 
    or (dtyf > 1.f)
    or (dtzf > 1.f)
    // useless ?
    or (dtentry >= dtexit) // allow for getting out of the insides of a box
    or ((dtxf < 0.f) and (dtyf < 0.f) and (dtzf < 0.f)) 
    )
    {
        // std::cout << "  > FALSE: " << std::endl;
        return false;
    }
    else // a collision happened
    {
        // collision response
        int i = math::maxindex(dtxf, dtyf, dtzf);
        
        // only restraint one axis
        if (i == 1) 
        {
            if (almostZero(v.y) and yout) [[unlikely]] // edge fix
            {
                // std::cout << "  > FALSE: revaluated" << std::endl;
                return false;
            }
            
            // a collision has happened
            v.x *= (dtentry);
            mv.velocity.x = 0;
            // std::cout << "  new v.x = " << v.x << std::endl;
        }
        else if (i == 2) 
        {
            if (almostZero(v.x) and xout) [[unlikely]] // edge fix
            {
                // std::cout << "  > FALSE: revaluated" << std::endl;
                return false;
            }
            
            // a collision has happened
            v.y *= (dtentry);
            mv.velocity.y = 0;
            // std::cout << "  new v.y = " << v.y << std::endl;
        }
        else if (i == 3) 
        {
            // TODO: BUG: allow traversing sometimes
            if ((almostZero(v.x) and xout) or (almostZero(v.y) and yout)) [[unlikely]] // edge fix
            {   
                std::cout << "------------------------" << std::endl;
                std::cout << "  step: x: " << v.x << ", y: " << v.y << ", z: " << v.z << std::endl;
                std::cout << "  out: " << xout << " " << yout << " " << zout << std::endl;
                std::cout << "  zro: " << almostZero(v.x) << " " << almostZero(v.y) << " " << almostZero(v.z) << std::endl;
                std::cout << "  dt: " << dtentry    << "  \t\t" << dtexit << std::endl;
                std::cout << "  delta: " << v.x     << "  \t\t" << v.y    << "\t\t" << v.z << std::endl;
                std::cout << "  dtf: " << dtxf      << "  \t\t" << dtyf   << "\t\t" << dtzf << "\t" << std::endl;

                std::cout << "  > FALSE: revaluated" << std::endl;
                return false;
            }
            
            // a collision has happened
            v.z *= (dtentry);
            mv.velocity.z = 0;
            // std::cout << "  new v.z = " << v.z << std::endl;
        }
        
        // std::cout << "  > TRUE:  " << std::endl;
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
        if (ecs::component::get<Movement>(e1).delta == sf::Vector3f(0,0,0)) // e1 is cappable of moving, but is static
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
                // std::cout << "[[[ " << e1 << " against " << e2 << " ]]]" <<std::endl;
                sweptAABB(h1, mv, h2, dist);
            }   
        }
    }
}