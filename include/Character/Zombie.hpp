#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include "Character/Mob.hpp"
#include "UI/EnemyHP.hpp"

class Zombie : public Mob{
public:
    Zombie(std::vector<std::string>& path, int Hp, std::shared_ptr<Player> player, GameWorldContext& World);
    ~Zombie();

    void Attacked(int Damage, glm::vec2 Dir, float Velocity = 0.0f) override;
    
    void Update(float dt) override;

    int GetScore() {return m_score;}
private:
    void Attack(float dt) override;

    void Move(float dt) override;

private:
    std::shared_ptr<EnemyHP> m_HpUI;
};


#endif