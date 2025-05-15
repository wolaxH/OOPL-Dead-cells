#ifndef TWINDAGGERS_HPP
#define TWINDAGGERS_HPP

#include "Item/Weapon/Weapon.hpp"


class TwinDaggers : public Weapon{
public:
    TwinDaggers();
    ~TwinDaggers() = default;

    void Use(std::shared_ptr<Mob>& mob, const glm::vec2& Dir,int combo = 0) override;

    Rect GetHitBox(const glm::vec2& Pos, const glm::vec2& Dir) override;

private:
};

#endif