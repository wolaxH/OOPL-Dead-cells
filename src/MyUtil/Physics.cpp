#include "MyUtil/Physics.hpp"

bool Physics::IsOnGround(glm::vec2 pos, 
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

void Physics::ApplyGravity(float& velocityY, bool onGround, float gravity, float maxFallSpeed) {
    if (!onGround) {
        velocityY -= gravity;
        if (velocityY < -maxFallSpeed)
            velocityY = -maxFallSpeed;
    } else {
        if (velocityY < 0)
            velocityY = 0;
    }
}

void Physics::SlowDown(float& VelocityX, const float Friction){
    if (VelocityX > 0){
        VelocityX -= Friction;
        if (VelocityX < 0) VelocityX = 0;
    }
    else if(VelocityX < 0){
        VelocityX += Friction;
        if (VelocityX > 0) VelocityX = 0;
    }
}