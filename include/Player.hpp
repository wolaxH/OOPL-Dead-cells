#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"

extern std::vector<std::shared_ptr<Character>> Mobs;


class Player : public Character{
public:
    Player(std::vector<std::string>& path, int Hp);
    ~Player() noexcept = default;

    void Attack() override;

    void Update() override;
    
private:

    //player util
    bool InGround();
    void InitState(c_state state, const std::vector<std::size_t>& frames = {}, const std::vector<std::string>& paths = {});
    
    //for update pos
    void applyGravity();
    void FixPos() override;
    void Move();
    void Jump();
private:
    float VelocityX = 0, VelocityY = 0; //speed
    float AccelerationX = 2;    //加速度
    float Friction = 1.5f;   //摩擦力(減速速度)
    float MaxSpeed = 15.0f;
    float Gravity = 0.8f;
    float MaxFallSpeed = 20.0f;
    int jumpStep = 0;   //double jump counter
};
#endif