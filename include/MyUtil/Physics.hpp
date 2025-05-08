#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "pch.hpp"
#include "SolidObj.hpp"
#include "OneSidedPlatform.hpp"

namespace Physics {

    inline void ApplyGravity(float& velocityY, bool onGround, float gravity, float maxFallSpeed) {
        if (!onGround) {
            velocityY -= gravity;
            if (velocityY < -maxFallSpeed)
                velocityY = -maxFallSpeed;
        } else {
            if (velocityY < 0)
                velocityY = 0;
        }
    }



    inline bool IsOnGround(glm::vec2 pos, 
        const std::vector<std::shared_ptr<SolidObj>>& solids, 
        const std::vector<std::shared_ptr<OneSidedPlatform>>& platforms) {
        glm::vec2 other_Pos, other_scale;
        float x, y;

        std::vector<std::shared_ptr<SolidObj>> temp(solids);
        temp.insert(temp.end(), platforms.begin(), platforms.end());

        for (const auto& Solid : temp) {
            other_Pos = Solid->m_WorldPos;
            other_scale = abs(Solid->GetScaledSize());

            x = !((pos.x < other_Pos.x - other_scale.x / 2 - 1) ||
                    (pos.x > other_Pos.x + other_scale.x / 2 + 1));
            y = (pos.y > other_Pos.y - other_scale.y / 2) &&
                (pos.y < other_Pos.y + other_scale.y / 2 + 2);
            if (x && y) return true;
        }

        return false;
    }

    
}

#endif