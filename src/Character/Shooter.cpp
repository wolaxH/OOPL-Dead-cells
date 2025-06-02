#include "Character/Shooter.hpp"


Shooter::Shooter(std::vector<std::string>& path, int Hp, std::shared_ptr<Player> player, GameWorldContext& World) 
    : Mob(path, Hp, player, World){
    top = 50, bottom = 52, left = 13, right = 13;
    m_DetectRange = 600.0f;
    m_Transform.translation = {1.0f, 1.0f};
    m_AtkRange = 500.0f;
    m_AtkCoolDownTimer.SetTime(2000, 2000);
    m_AtkPoint = 20;
}

void Shooter::Attacked(int Damage, glm::vec2 Dir, float Velocity){}

void Shooter::Attack(float dt){}

void Shooter::Move(float dt){

    if (m_state == mob_state::wander){
        
    }


}

void Shooter::LookAtPlayer(){
    int side = 1;
    if (m_player->m_WorldPos.x < m_WorldPos.x) side = -1;
    m_Transform.scale.x = side * std::abs(m_Transform.scale.x);
}

void Shooter::Update(float dt){


    InGround = Physics::IsOnGround(m_WorldPos, m_World.SolidObjs, m_World.OneSidedPlatforms);
    Physics::ApplyGravity(VelocityY, InGround, Gravity, MaxFallSpeed);
    if (InGround) Physics::SlowDown(VelocityX, Friction);

    if (IsNearBy(m_player, m_DetectRange)){
        LookAtPlayer();
        if (IsNearBy(m_player, m_AtkRange)){
            m_state = mob_state::trace;

            if (m_AtkCoolDownTimer.IsTimeout()){
                Attack(dt);
                m_AtkCoolDownTimer.ResetTime(); //reset cooldown
            }
            else{   //aproach player
                Move(dt);
            }
        }
        else{
            m_state = mob_state::wander;
            Move(dt);
        }
    }

    PushPlayer();
    FixPos(dt);

    m_WorldPos += glm::vec2(VelocityX, VelocityY) * dt;
}