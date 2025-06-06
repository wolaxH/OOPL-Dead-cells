#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "pch.hpp"
#include "SolidObj.hpp"
#include "OneSidedPlatform.hpp"
#include "Abstract/MapObj.hpp"

namespace Physics {
    void SlowDown(float& VelocityX, const float Friction);
    
    void ApplyGravity(float& velocityY, bool onGround, float gravity, float maxFallSpeed);
        
    bool IsOnGround(const MapObj* Obj, 
        const std::vector<std::shared_ptr<SolidObj>>& solids, 
        const std::vector<std::shared_ptr<OneSidedPlatform>>& platforms);
}

#endif