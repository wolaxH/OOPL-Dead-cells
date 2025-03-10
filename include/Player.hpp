#include "Character.hpp"
#include "Ground.hpp"

extern std::vector<std::shared_ptr<Ground>> grounds;

class Player : public Character{
public:
    Player(std::vector<std::string>& path, int Hp);
    ~Player() noexcept = default;

    void FixPos(std::shared_ptr<Util::GameObject> other) override;

    void Update(std::shared_ptr<Util::GameObject> other) override;

    bool InGround();

    void applyGravity();

private:
    void Move();
    void Jump();
    float VelocityX = 0, VelocityY = 0; //speed
    float AccelerationX = 2;    //加速度
    float Friction = 0.8f;   //摩擦力(減速速度)
    float MaxSpeed = 10.0f;
    float Gravity = 0.5f;
    float MaxFallSpeed = 10.0f;
};