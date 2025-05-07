#ifndef RUSTYSWORD_HPP
#define RUSTYSWORD_HPP

#include "Item/Weapon/Weapon.hpp"

class RustySword : public Weapon{
public:
    RustySword();
    ~RustySword() = default;

    void Use(const glm::vec2& Pos, const glm::vec2& Dir) override;

    Rect GetHitBox(const glm::vec2& Pos, const glm::vec2& Dir) override;

private:
   Rect HitBox;
};


#endif // RUSTYSWORD_HPP_HPP