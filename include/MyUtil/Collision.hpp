#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Abstract/MapObj.hpp"

namespace Collision{
    bool IsIntersect(const MapObj* a, const MapObj* b);

    struct AABB {
        float left, top, right, bottom;
    };
    bool IsIntersectAABB(const AABB& a, const AABB& b);

    AABB GetAABB(const MapObj* obj);
    
} // namespace Collision

#endif