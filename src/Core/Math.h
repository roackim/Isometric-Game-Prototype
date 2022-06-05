#pragma once

#include <SFML/System.hpp>
#include <cmath>

namespace math
{
bool sign(float f);

float abs(float f);

float dist(const sf::Vector3f& a, const sf::Vector3f& b);

void swap(float& a, float&b);

float max(const float a, const float b);

float min(const float a, const float b);

float closestToZero(const float a, const float b);

float maxindex(const float a, const float b);

float max(const float a, const float b, const float c);

float min(const float a, const float b, const float c);

float maxindex(const float a, const float b, const float c);

float CosAngle(sf::Vector2f v, sf::Vector2f k);

float dotProduct(sf::Vector2f v, sf::Vector2f k);

float norm(sf::Vector2f v);

}