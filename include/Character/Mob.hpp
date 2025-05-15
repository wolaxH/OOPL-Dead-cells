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
    : Character(path, Hp, World), player(player){MaxSpeed = 5.0f, AccelerationX = 1.5f;}
    virtual ~Mob() = default;

protected:
    virtual bool IsPlayerNearby() = 0;

    void PushPlayer(){
        auto obj = std::static_pointer_cast<MapObj>(player);
        if (Collision::IsIntersect(this, player.get())){
            player->Pushed();            
        }    
    }

protected:
    bool AtkFlag = false;
    float DetectRange;
    mob_state m_state = mob_state::wander;
    std::shared_ptr<Player> player;

};


#endif