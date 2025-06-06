#ifndef RUSTYSWORD_HPP
#define RUSTYSWORD_HPP

#include "Item/Weapon/Abstract/Weapon.hpp"

class RustySword : public Weapon{
public:
    RustySword();
    ~RustySword() = default;

    void Use(std::shared_ptr<Mob>& mob, const glm::vec2& Dir = {1, 1}, int combo = 0) override;

    Rect GetHitBox(const glm::vec2& Pos, const glm::vec2& Dir, int combo = 0) override;

private:
};


#endif // RUSTYSWORD_HPP