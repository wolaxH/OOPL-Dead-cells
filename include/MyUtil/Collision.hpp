#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Abstract/MapObj.hpp"

namespace Collision{
    bool IsIntersect(const MapObj* a, const MapObj* b);
    
} // namespace Collision

#endif