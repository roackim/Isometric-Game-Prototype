#include "Collision.h"
#include "SECS/ECS.hpp"


#include "Components/Movement.hpp"
#include "Core/Math.h"

static bool sweptAABB(Hitbox& a, sf::Vector3f& v, const Hitbox& b, float& dist);

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
        auto& mv = ecs::component::get<Movement>(e1);
        auto& h1  = ecs::component::get<Hitbox>(e1);
        for (uint e2 : v2)
        {
            auto& h2  = ecs::component::get<Hitbox>(e2);
            
            if (ecs::entity::has<Movement>(e2)) // both entities are moving
            {
                // TODO properly 
                bool res = sweptAABB(h1, mv.delta, h2, dist);
            }
            else // second entity is static
            {
                sweptAABB(h1, mv.delta, h2, dist);
            }   
        }
    }
}



// A is tested against B, this assumes B is static
static bool sweptAABB(Hitbox& a, sf::Vector3f& v, const Hitbox& b, float& dist)
{
    float dxf, dyf, dzf; // difference, between nearest edges 
    float dxb, dyb, dzb; // difference, between furthest edges 
    
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
    
    // avoid division by 0              // std::numeric_limits<T>::epsilon()
    if (v.x == 0.f) v.x = math::sign(v.x) * 0.000000001f;
    if (v.y == 0.f) v.y = math::sign(v.y) * 0.000000001f;
    if (v.z == 0.f) v.z = math::sign(v.z) * 0.000000001f;
    
    // compute time of collision per axis
    dtxf = dxf / v.x; // x
    dtxb = dxb / v.x;   
    
    dtyf = dyf / v.y; // y
    dtyb = dyb / v.y;
    
    dtzf = dzf / v.z; // z
    dtzb = dzb / v.z;
    
    // determine where the constraint comes from
    float dtentry = math::max(dtxf, dtyf, dtzf);
    float dtexit  = math::min(dtxb, dtyb, dtzb);
    
    // cases where there is no collision
    if ((dtentry >= dtexit) 
    or ((dtxf < 0.f) and (dtyf < 0.f) and (dtzf < 0.f)) 
    or (dtxf > 1.f) // going to collide but not this frame 
    or (dtyf > 1.f)
    or (dtzf > 1.f)
    )
    {
        return false;
    }
    else // a collision happened
    {
        std::cout << "------------------------" << std::endl;
        std::cout << "dt: " << dtentry << "\t" << dtexit << std::endl;
        std::cout << "ds: " << dxf << "\t" << dyf << "\t" << dzf << std::endl;
        std::cout << "vel: " << v.x << "\t" << v.y << "\t" << v.z << std::endl;
        std::cout << dtxf << "\t" << dtyf << "\t" << dtzf << "\t";
        // std::cout << dtentry << std::endl;
        // collision response
        int i = math::maxindex(dtxf, dtyf, dtzf);
        
        // only restraint one axis
        if      (i == 1) 
        {
            v.x *= (dtentry);// - 0.001f); // - 0.0001 -> avoid staying in collision state after resolution
            std::cout << "New v.x " << v.x << std::endl;
        }
        else if (i == 2) 
        {
            v.y *= (dtentry);// - 0.001f);
            std::cout << "New v.y " << v.y << std::endl;
        }
        else if (i == 3) 
        {
            v.z *= (dtentry);// - 0.001f);
            std::cout << "New v.z " << v.z << std::endl;
        }
        
        return true;
    }
}