#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include "Character/Mob.hpp"
#include "MyUtil/Timer.hpp"

#include <ctime>

class Zombie : public Mob{
public:
    Zombie(std::vector<std::string>& path, int Hp, std::shared_ptr<Player> player, GameWorldContext& World) 
        : Mob(path, Hp, player, World){
        top = 50, bottom = 52, left = 13, right = 13;
        DetectRange = 300.0f;
        m_Transform.translation = {1.0f, 1.0f};
        AtkRange = 20.0f;
    }
    ~Zombie() = default;

    void Attack(float dt) override;

    void Attacked(int Damage, glm::vec2 Dir) override;

    void Update(float dt) override;

private:
    void Move(float dt) override;
    bool IsPlayerNearby() override;

private:
    int AtkPoint;
    Timer timer;
};


#endif