#ifndef MOB_HPP
#define MOB_HPP

#include "Character/Character.hpp"
#include "Character/Player.hpp"

enum class mob_state{
    trace,
    wander
};

class Mob : public Character{
public:
    Mob(std::vector<std::string>& path, int Hp, std::shared_ptr<Player> player, GameWorldContext& World)
    : Character(path, Hp, World), m_player(player){MaxSpeed = 5.0f, AccelerationX = 1.5f;}
    virtual ~Mob() = default;

protected:

    void PushPlayer(){
        auto obj = std::static_pointer_cast<MapObj>(m_player);
        if (Collision::IsIntersect(this, m_player.get())){
            m_player->Pushed();            
        }    
    }

protected:
    float m_AtkRange;
    bool m_AtkFlag = false;
    float m_DetectRange;
    mob_state m_state = mob_state::wander;
    std::shared_ptr<Player> m_player;

};


#endif