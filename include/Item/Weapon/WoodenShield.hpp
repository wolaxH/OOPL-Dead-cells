#ifndef WOODENSHIELD_HPP
#define WOODENSHIELD_HPP

#include "Item/Weapon/WeaponUtil/Weapon.hpp"
#include "Item/Weapon/WeaponUtil/Shield.hpp"

class WoodenShield : public Shield {
public:
    WoodenShield();
    ~WoodenShield() = default;

    void Parry(std::shared_ptr<Mob>& mob, const glm::vec2& Dir = {1, 1}, int combo = 0) override;

private:

};

#endif