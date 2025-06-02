#ifndef SHOOTER_HPP
#define SHOOTER_HPP

#include "Character/Mob.hpp"
#include "MyUtil/Projectile.hpp"
#include "MyUtil/Timer.hpp"

class Shooter : public Mob {
public:
    Shooter(std::vector<std::string>& path, int Hp, std::shared_ptr<Player> player, GameWorldContext& World);
    ~Shooter() = default;

    void Update(float dt) override;

    void Attacked(int Damage, glm::vec2 Dir, float Velocity = 0.0f) override;

    

private:
    void LookAtPlayer();

    void Attack(float dt) override;

    void Move(float dt) override;
private:
    Timer m_Timer;
};

#endif