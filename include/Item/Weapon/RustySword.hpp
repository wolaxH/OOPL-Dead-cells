#ifndef RUSTYSWORD_HPP
#define RUSTYSWORD_HPP

#include "Item/Weapon/Weapon.hpp"

class RustySword : public Weapon{
public:
    RustySword();
    ~RustySword() = default;

    void Use(std::shared_ptr<Mob>& mob) override;

    Rect GetHitBox(const glm::vec2& Pos, const glm::vec2& Dir) override;

private:
};


#endif // RUSTYSWORD_HPP_HPP