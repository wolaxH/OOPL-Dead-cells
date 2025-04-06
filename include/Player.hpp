#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
//#include "Mob.hpp"

//extern std::vector<std::shared_ptr<Mob>> Mobs;


class Player : public Character{
public:
    Player(std::vector<std::string>& path, int Hp);
    ~Player() noexcept = default;

    void Attack() override;

    void Update() override;

    void Attacked(int damage){Hp -= damage;}

    //for mob push
    void Pushed(){ 
        if (VelocityX > 0){
            VelocityX *= 0.5f;
            if (VelocityX < 5) VelocityX = 5.0f;
        }
        else if (VelocityX < 0){
            VelocityX *= 0.5f;
            if (VelocityX > -5) VelocityX = -5.0f;
        }
    }
    
private:

    void Clinb();   //攀爬
    void Move() override;
    void Jump();

private:
    
    int jumpStep = 0;   //double jump counter
};
#endif