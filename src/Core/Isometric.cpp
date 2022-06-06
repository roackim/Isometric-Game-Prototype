#include "Isometric.h"

#include <SFML/System.hpp>

sf::Vector2f isometric::getProjection(sf::Vector3f v)
{ 
    return v.x * isometric::vux + v.y * isometric::vuy + v.z * isometric::vuz;
}
