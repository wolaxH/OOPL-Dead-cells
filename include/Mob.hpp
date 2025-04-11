#ifndef MOB_HPP
#define MOB_HPP

#include "Character.hpp"
#include "Player.hpp"

enum class mob_state{
    trace,
    wander
};

class Mob : public Character{
public:
    Mob(std::vector<std::string>& path, int Hp, std::shared_ptr<Player> player, const std::vector<std::shared_ptr<SolidObj>>& SolidObjs)
    : Character(path, Hp, SolidObjs), player(player){MaxSpeed = 5.0f, AccelerationX = 1.5f;}
    ~Mob() = default;

protected:
    virtual bool IsPlayerNearby() = 0;

    void PushPlayer(){
        if (IsCollsion(player)){
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