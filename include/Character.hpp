#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Util/Animation.hpp"

#include "MapObj.hpp"
#include "SolidObj.hpp"

#include <unordered_map>

extern std::vector<std::shared_ptr<SolidObj>> SolidObjs;

enum class c_state{ //Character state
    idle,
    L_move,
    R_move,
    jump,
    fall,
    atk,
    clinb
};

class Character : public MapObj{
public:    
    Character(std::vector<std::string>& path, int Hp);
    ~Character() noexcept = default;

    void SetPos(glm::vec2 pos){ m_WorldPos = pos;}

    void virtual Update() = 0;

    void SetState(c_state State, std::vector<std::string> path = {}, bool loop = true);

    c_state GetState(){ return State;}

    bool IsCollsion(std::shared_ptr<MapObj> other);

    void virtual Attack() = 0;


protected:
    bool IsContainState(c_state State){return (D_Manager.find(State) != D_Manager.end()) ? true : false;}

    //player util
    bool InGround();
    /*
        @param path eg:RESOURCE_DIR"Zombie/move/move_"
    */
    void InitState(c_state state, const std::vector<std::size_t>& frames = {}, const std::vector<std::string>& paths = {});
    
    //for update pos
    void applyGravity();
    void FixPos();
    virtual void Move() = 0;

protected:
    float VelocityX = 0, VelocityY = 0; //speed
    float MaxSpeed = 15.0f;
    float AccelerationX = 2;    //加速度
    const float Friction = 1.5f;   //摩擦力(減速速度)
    const float Gravity = 0.8f;
    const float MaxFallSpeed = 20.0f;


    float AtkRange;
    int Hp;

private:
    std::unordered_map<c_state, std::shared_ptr<Core::Drawable>> D_Manager;  //Drawable Manager
    c_state State;      //current state
};

#endif