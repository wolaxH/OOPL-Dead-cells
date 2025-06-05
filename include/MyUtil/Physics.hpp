#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "pch.hpp"
#include "SolidObj.hpp"
#include "OneSidedPlatform.hpp"

namespace Physics {
    void SlowDown(float& VelocityX, const float Friction);
    
    void ApplyGravity(float& velocityY, bool onGround, float gravity, float maxFallSpeed);
        
    bool IsOnGround(glm::vec2 pos, 
        const std::vector<std::shared_ptr<SolidObj>>& solids, 
        const std::vector<std::shared_ptr<OneSidedPlatform>>& platforms);
}

#endif