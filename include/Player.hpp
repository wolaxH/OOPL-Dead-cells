#include "Character.hpp"

class Player : public Character{
public:
    Player(std::vector<std::string>& path, int Hp);

    void Update() override;

private:
    void Move();
    void Jump();
    float VelocityX = 0, VelocityY = 0; //speed
    float AccelerationX = 1;    //加速度
    float Friction = 0.8f;   //摩擦力(減速速度)
    float MaxSpeed = 8.0f;
};