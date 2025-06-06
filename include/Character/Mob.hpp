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
    : Character(path, Hp, World), m_player(player){MaxSpeed = 5.0f, AccelerationX = 2.0f;}
    virtual ~Mob() = default;

protected:
    bool IsSameLevelNearBy(std::shared_ptr<MapObj> other, float distance = 1000.0f){
        glm::vec2 D = m_WorldPos - other->m_WorldPos;
        return (std::abs(D.y) < 100.0f && glm::length(D) <= distance);
    }

    void PushPlayer();

    void Wander(const std::string& path, const size_t frames);
protected:
    float m_AtkRange;
    bool m_AtkFlag = false;
    float m_DetectRange;
    int m_AtkPoint;
    
    Timer m_AtkCoolDownTimer;

    mob_state m_state = mob_state::wander;
    std::shared_ptr<Player> m_player;
private:
    Timer m_WanderTimer;
};
#endif