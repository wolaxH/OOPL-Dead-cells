#ifndef BONE_HPP
#define BONE_HPP

#include "Item/Weapon/Abstract/Weapon.hpp"

class Bone : public Weapon{
public:
    Bone();
    ~Bone() noexcept = default;

    void Use(std::shared_ptr<Mob>& mob, const glm::vec2& Dir = {1, 1}, int combo = 0) override;

    Rect GetHitBox(const glm::vec2& Pos, const glm::vec2& Dir, int combo = 0) override;

};

#endif