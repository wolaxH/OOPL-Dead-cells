#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include "Mob.hpp"
#include "Timer.hpp"

#include <ctime>

class Zombie : public Mob{
public:
    Zombie(std::vector<std::string>& path, int Hp, std::shared_ptr<Player> player, const std::vector<std::shared_ptr<SolidObj>>& SolidObjs) 
    : Mob(path, Hp, player, SolidObjs){
        top = 50, bottom = 52, left = 13, right = 13;
        DetectRange = 300.0f;
        m_Transform.translation = {1.0f, 1.0f};
        AtkRange = 20.0f;
        
    }
    ~Zombie() = default;

    void Update() override;
    void Attack() override;

private:
    void Move() override;
    bool IsPlayerNearby() override;

private:
    int AtkPoint;
    Timer timer;
};


#endif