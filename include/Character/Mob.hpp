#ifndef MOB_HPP
#define MOB_HPP

#include "Character/Character.hpp"
#include "Character/Player.hpp"
#include "MyUtil/Timer.hpp"


class Mob : public Character{
public:
    enum class mob_state{
        trace,
        wander
    };

    Mob(std::vector<std::string>& path, int Hp, std::shared_ptr<Player> player, GameWorldContext& World)
    : Character(path, Hp, World), m_player(player){MaxSpeed = 5.0f, AccelerationX = 1.5f;}
    virtual ~Mob() = default;

protected:
    void PushPlayer();

    void Wander(const std::string& path);
protected:
    float m_AtkRange;
    bool m_AtkFlag = false;
    float m_DetectRange;
    int m_AtkPoint;
    
    Timer m_AtkCoolDownTimer;
    Timer m_WanderTimer;

    mob_state m_state = mob_state::wander;
    std::shared_ptr<Player> m_player;
};
#endif