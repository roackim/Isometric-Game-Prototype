#include "Math.h"


#include <SFML/System.hpp>
#include <cmath>

namespace math
{

    bool signbool(float f)
    {
        return f >= 0.f;   
    }
    
    float sign(float f)
    {
        if (f >= 0.f) return 1.f;
        return -1.f;   
    }

    float abs(float f)
    {
        if (f < 0) return -f;
        return f;  
    }

    float dist(const sf::Vector3f& a, const sf::Vector3f& b)
    {
        sf::Vector3f d = b - a;
        return std::sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
    }

    void swap(float& a, float&b)
    {
        float c = b;
        b = a;
        a = c;   
    }

    float max(const float a, const float b)
    {
        if (a > b) return a; 
        return b; 
    }

    float min(const float a, const float b)
    {
        if (a < b) return a; 
        return b; 
    }
    
    float closestToZero(const float a, const float b)
    {
        if (abs(a) < abs(b)) return a; 
        return b; 
    }

    float maxindex(const float a, const float b)
    {
        if (a > b) return 1; 
        return 2; 
    }

    float max(const float a, const float b, const float c)
    {
        if      (a >= b and a >= c) return a;
        else if (b >= a and b >= c) return b;
        return c;
    }

    float min(const float a, const float b, const float c)
    {
        if      (a <= b and a <= c) return a;
        else if (b <= a and b <= c) return b;
        return c;
    }

    float maxindex(const float a, const float b, const float c)
    {
        if      (a >= b and a >= c) return 1;
        else if (b >= a and b >= c) return 2;
        return 3;
    }


    float CosAngle(sf::Vector2f v, sf::Vector2f k)
    {
        float s = norm(v) * norm(k);
        if (s == 0) return 0;
        return dotProduct(v, k) / s;
    }

    float dotProduct(sf::Vector2f v, sf::Vector2f k)
    {
        return v.x * k.x + v.y * k.y;   
    }

    float norm(sf::Vector2f v)
    {
        return std::sqrt(v.x*v.x + v.y*v.y);   
    }
}