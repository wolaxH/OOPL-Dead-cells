#include "MyUtil/Physics.hpp"
#include "MyUtil/Collision.hpp"

bool Physics::IsOnGround(const MapObj* Obj, 
        const std::vector<std::shared_ptr<SolidObj>>& solids, 
        const std::vector<std::shared_ptr<OneSidedPlatform>>& platforms) {

    Collision::AABB aabb = Collision::GetAABB(Obj);

    // 檢查 SolidObj
    for (const auto& Solid : solids) {
        Collision::AABB solidAABB = Collision::GetAABB(Solid.get());

        // 稍微放寬 y 軸底部接觸範圍 (+1 容錯)
        bool overlapX = aabb.right > solidAABB.left && aabb.left < solidAABB.right;
        bool onTop = std::abs(aabb.bottom - solidAABB.top) <= 3.5f;

        if (overlapX && onTop) {
            return true;
        }
    }

    // 檢查 OneSidedPlatform
    for (const auto& OSP : platforms) {
        Collision::AABB ospAABB = Collision::GetAABB(OSP.get());

        // 角色底部要接觸平台頂部，並且角色要在平台上方落下來
        bool overlapX = aabb.right > ospAABB.left && aabb.left < ospAABB.right;
        bool onTop = std::abs(aabb.bottom - ospAABB.top) <= 3.5f;
        // bool isAbove = aabb.bottom >= ospAABB.top;

        if (overlapX && onTop) {
            return true;
        }
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