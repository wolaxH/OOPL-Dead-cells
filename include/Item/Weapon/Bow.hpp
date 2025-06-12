#ifndef BOW_HPP
#define BOW_HPP

#include "Item/Weapon/Abstract/Weapon.hpp"
#include "Item/Weapon/Abstract/IShootable.hpp"
#include "MyUtil/GameWorldContext.hpp"
#include "MyUtil/Projectile.hpp"


class Bow : public Weapon, public ISootable{
public:
    Bow(GameWorldContext& ctx);
    ~Bow() = default;

    void Use(std::vector<std::shared_ptr<GameObject>>& Objs, const glm::vec2& Pos, bool& UsedFlag, const glm::vec2& Dir = {1, 1}, int combo = 0) override;

    void Shoot(const glm::vec2& Dir, const glm::vec2& Pos) override;

private:
    GameWorldContext& m_World;
};

#endif