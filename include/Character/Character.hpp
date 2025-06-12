#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Util/Animation.hpp"

#include "Abstract/MapObj.hpp"
#include "MyUtil/GameWorldContext.hpp"
#include "MyUtil/AccessKey.hpp"
#include "MyUtil/Physics.hpp"
#include "MyUtil/Collision.hpp"


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
    crouch,
    roll,
    atked,
    block,
    heal,
    death
};

class Character : public MapObj{
public:    
    Character(std::vector<std::string>& path, int Hp, GameWorldContext& World);
    virtual ~Character() = default;

    void SetPos(glm::vec2 pos){ m_WorldPos = pos;}

    void virtual Update(float dt) = 0;
    
    c_state GetState(){ return State;}
    
    /**
     * Attacked will end by itself
     * so if current state = atked and there is no special method can interupt it(like roll)
     * then let It End
     */
    void virtual Attacked(int Damage, glm::vec2 Dir, float Velocity = 0.0f) = 0;
    
    bool IsAlive() const noexcept{ return m_Hp > 0;}
    
protected:
    void SetState(c_state State, std::vector<std::string> path = {}, bool Isloop = true);

    bool IsContainState(c_state State){return (D_Manager.find(State) != D_Manager.end()) ? true : false;}
    
    /**
     * @param path eg:RESOURCE_DIR"Zombie/move/move_"
     */
    void InitState(c_state state, const std::vector<std::size_t>& frames = {}, const std::vector<std::string>& paths = {});
    
    void InitState(c_state State, std::shared_ptr<Core::Drawable> drawable);
    
    /*用來修正位置，使其不會穿牆，不修改c_state*/
    void FixPos(float dt);

    /**
     * Same as Attacked
     */
    void virtual Attack(float dt) = 0;

    //移動
    virtual void Move(float dt) = 0;

    bool IsNearBy(std::shared_ptr<MapObj> other, float distance = 1000.0f);

    bool IsNearBy(glm::vec2 Point, float distance = 1000.0f);

    void ChangeDrawable(AccessKey key, std::shared_ptr<Core::Drawable> PlayerDrawable, c_state state = c_state::atk);

protected:
    float VelocityX = 0, VelocityY = 0; //speed
    float MaxSpeed = 12.0f;
    float AccelerationX = 2;    //加速度
    const float Friction = 1.5f;   //摩擦力(減速速度)
    const float Gravity = 0.8f;
    const float MaxFallSpeed = 20.0f;

    int m_Hp;
    bool InGround;
    GameWorldContext& m_World;   //reference of World resource
    bool m_IgnoreOSP = false;

private:
    std::unordered_map<c_state, std::shared_ptr<Core::Drawable>> D_Manager;  //Drawable Manager
    c_state State;      //current state

};

#endif