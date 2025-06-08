#ifndef BONE_HPP
#define BONE_HPP

#include "Item/Weapon/Abstract/Weapon.hpp"

class Bone : public Weapon{
public:
    Bone();
    ~Bone() noexcept = default;

    void Use(std::vector<std::shared_ptr<GameObject>>& Mobs, const glm::vec2& Pos, bool& UsedFlag, const glm::vec2& Dir = {1, 1}, int combo = 0) override;

    Rect GetHitBox(const glm::vec2& Pos, const glm::vec2& Dir, int combo = 0) override;

};

#endif