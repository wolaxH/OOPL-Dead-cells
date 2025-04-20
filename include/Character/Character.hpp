#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Util/Animation.hpp"

#include "Abstract/MapObj.hpp"
#include "SolidObj.hpp"
#include "OneSidedPlatform.hpp"

#include <unordered_map>
#include <algorithm>

enum class c_state{ //Character state
    idle,
    L_move,
    R_move,
    jump,
    fall,
    atk,
    clinb,
    clinbOSP,
    roll
};

class Character : public MapObj{
public:    
    Character(  std::vector<std::string>& path, int Hp,
                const std::vector<std::shared_ptr<SolidObj>>& SolidObjs, 
                const std::vector<std::shared_ptr<OneSidedPlatform>>& OSP);
    ~Character() noexcept = default;

    void SetPos(glm::vec2 pos){ m_WorldPos = pos;}

    void virtual Update() = 0;

    void SetState(c_state State, std::vector<std::string> path = {}, bool Isloop = true);

    c_state GetState(){ return State;}

    bool IsCollsion(std::shared_ptr<MapObj> other);

    void virtual Attack() = 0;


protected:
    bool IsContainState(c_state State){return (D_Manager.find(State) != D_Manager.end()) ? true : false;}

    bool InGround();

    /**
     * @param path eg:RESOURCE_DIR"Zombie/move/move_"
     */
    void InitState(c_state state, const std::vector<std::size_t>& frames = {}, const std::vector<std::string>& paths = {});
    


    /**
     * only for set VelocityY，不修改c_state
     */
    void applyGravity();

    /*用來修正位置，使其不會穿牆，不修改c_state*/
    void FixPos();

    //移動
    virtual void Move() = 0;

protected:
    float VelocityX = 0, VelocityY = 0; //speed
    float MaxSpeed = 12.0f;
    float AccelerationX = 2;    //加速度
    const float Friction = 1.5f;   //摩擦力(減速速度)
    const float Gravity = 0.8f;
    const float MaxFallSpeed = 20.0f;


    float AtkRange;
    int Hp;
    const std::vector<std::shared_ptr<SolidObj>>& r_SolidObjs; //reference of SolidObjs
    const std::vector<std::shared_ptr<OneSidedPlatform>>& r_OneSidedPlatforms; //reference of OneSidedPlatforms

private:
    std::unordered_map<c_state, std::shared_ptr<Core::Drawable>> D_Manager;  //Drawable Manager
    c_state State;      //current state
};

#endif