#ifndef TWINDAGGERS_HPP
#define TWINDAGGERS_HPP

#include "Item/Weapon/Abstract/Weapon.hpp"


class TwinDaggers : public Weapon{
public:
    TwinDaggers();
    ~TwinDaggers() = default;

    void Use(std::vector<std::shared_ptr<GameObject>>& Mobs, const glm::vec2& Pos, bool& UsedFlag, const glm::vec2& Dir = {1, 1}, int combo = 0) override;

    Rect GetHitBox(const glm::vec2& Pos, const glm::vec2& Dir, int combo = 0) override;

private:
};

#endif