#pragma once

#include <SFML/System.hpp>
#include "Core/Vector.h"
#include <cmath>

namespace math
{
bool signbool(float f);

float sign(float f);

float abs(float f);

float dist(const v3f& a, const v3f& b);

void swap(float& a, float&b);

float max(const float a, const float b);

float min(const float a, const float b);

float closestToZero(const float a, const float b);

float maxindex(const float a, const float b);

float max(const float a, const float b, const float c);

float min(const float a, const float b, const float c);

float maxindex(const float a, const float b, const float c);

float CosAngle(v2f v, v2f k);

float dotProduct(v2f v, v2f k);

float norm(v2f v);

}