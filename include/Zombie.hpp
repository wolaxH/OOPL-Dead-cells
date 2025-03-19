#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include "Mob.hpp"

class Zombie : public Mob{
public:
    Zombie() = default;
    ~Zombie() = default;

    void Update() override;
    void Attack() override;


private:
    void move();
    void FixPos() override;
    int AtkPoint;

};


#endif