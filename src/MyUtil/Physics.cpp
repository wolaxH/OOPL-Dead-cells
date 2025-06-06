#include "MyUtil/Physics.hpp"

bool Physics::IsOnGround(const MapObj* Obj, 
        const std::vector<std::shared_ptr<SolidObj>>& solids, 
        const std::vector<std::shared_ptr<OneSidedPlatform>>& platforms) {
        glm::vec2 other_Pos;
        float x, y;

        std::vector<std::shared_ptr<SolidObj>> temp(solids);
        temp.insert(temp.end(), platforms.begin(), platforms.end());

        for (const auto& Solid : temp) {
            other_Pos = Solid->m_WorldPos;

            x = !(Obj->m_WorldPos.x < other_Pos.x - Solid->left - 1 ||
                    Obj->m_WorldPos.x > other_Pos.x + Solid->right + 1);
            y = (Obj->m_WorldPos.y + Obj->top > other_Pos.y + Solid->top) &&
                (Obj->m_WorldPos.y - Obj->bottom < other_Pos.y + Solid->top + 2);
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