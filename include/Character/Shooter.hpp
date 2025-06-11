#ifndef SHOOTER_HPP
#define SHOOTER_HPP

#include "Character/Mob.hpp"
#include "MyUtil/Projectile.hpp"
#include "UI/EnemyHP.hpp"

class Shooter : public Mob {
public:
    Shooter(std::vector<std::string>& path, int Hp, std::shared_ptr<Player> player, GameWorldContext& World);
    ~Shooter();

    void Update(float dt) override;

    void Attacked(int Damage, glm::vec2 Dir, float Velocity = 0.0f) override;

    void FireProjectile();

private:
    void Attack(float dt) override;

    void Move(float dt) override;
private:
    static constexpr int FIRE_FRAME = 21;
    std::shared_ptr<EnemyHP> m_HpUI;
};

#endif