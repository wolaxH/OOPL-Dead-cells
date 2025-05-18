#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include "Character/Mob.hpp"
#include "MyUtil/Timer.hpp"

#include <ctime>

class Zombie : public Mob{
public:
    Zombie(std::vector<std::string>& path, int Hp, std::shared_ptr<Player> player, GameWorldContext& World) ;
    ~Zombie() = default;

    void Attack(float dt) override;

    void Attacked(int Damage, glm::vec2 Dir) override;

    void Update(float dt) override;

private:
    void Move(float dt) override;

private:
    int AtkPoint;
    Timer timer;
};


#endif