#ifndef WOODENSHIELD_HPP
#define WOODENSHIELD_HPP

#include "Item/Weapon/WeaponUtil/Shield.hpp"

class WoodenShield : public Shield {
public:
    WoodenShield();
    ~WoodenShield() = default;

    void Parry(std::shared_ptr<Mob>& mob, const glm::vec2& Dir = {1, 1}) override;

private:
    int ParryDamage = 70;
};

#endif