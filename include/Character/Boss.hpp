#ifndef BOSS_HPP
#define BOSS_HPP

#include "Character/Mob.hpp"
#include "UI/BossHp.hpp"

#include <random>

struct AtkInfo{
    float AtkRange;
    int AtkPoint; 
    size_t FramesCount;
    //std::shared_ptr<Util::Animation> anim;
    std::string Path;
};

class Boss : public Mob{
public:
    Boss(std::vector<std::string>& path, int Hp, std::shared_ptr<Player> player, GameWorldContext& World);
    ~Boss() = default;

    void Attacked(int Damage, glm::vec2 Dir, float Velocity = 0.0f) override;
    
    void Update(float dt) override;
private:
    void Attack(float dt) override;

    void Move(float dt) override;
private:
    // boss Hp GameObject use
    std::shared_ptr<BossHp> m_HpUI;
    std::vector<AtkInfo> m_AtkInfo;
    //std::unordered_map<size_t, AtkInfo> m_AtkManager;
    int m_CurrentAtkID;
};

#endif