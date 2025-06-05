#include "Character/Zombie.hpp"

#include <random>

#include "Util/Logger.hpp"

Zombie::Zombie(std::vector<std::string>& path, int Hp, std::shared_ptr<Player> player, GameWorldContext& World) 
    : Mob(path, Hp, player, World){
        m_DetectRange = 500.0f;
        m_Transform.scale = {1.0f, 1.0f};
        m_AtkRange = 20.0f;
        m_AtkPoint = 30;
        m_AtkCoolDownTimer.SetTime(1000, 1500);
        top = 50 * m_Transform.scale.y;
        bottom = 52 * m_Transform.scale.y;
        left = 13 * m_Transform.scale.x;
        right = 13 * m_Transform.scale.x;
}

void Zombie::Attack(float dt){  //player
    if (GetState() == c_state::atked) return;

    //rendering 
    auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    if (temp->GetState() != Util::Animation::State::PLAY){
        m_AtkCoolDownTimer.ResetTime();
        SetState(c_state::idle);
        m_AtkFlag = false;
        return;
    }
    //rendering end

    if (m_AtkFlag) return;    //already attacked
    if (temp->GetCurrentFrameIndex() >= temp->GetFrameCount()-4 ||  //atkable frame only 18 ~ 24
        temp->GetCurrentFrameIndex() < 17 ) return; 

    //the real atk logic
    if (m_Transform.scale.x > 0){   //atk forward right
        if (m_player->m_WorldPos.x > m_WorldPos.x && m_player->m_WorldPos.x < m_WorldPos.x + m_AtkRange && m_player->IsAtkedable()){
            m_player->Attacked(m_AtkPoint, m_Transform.scale, 15.0f);
            m_AtkFlag = true;
        }
    }
    else{
        if (m_player->m_WorldPos.x < m_WorldPos.x && m_player->m_WorldPos.x > m_WorldPos.x - m_AtkRange && m_player->IsAtkedable()){
            m_player->Attacked(m_AtkPoint, m_Transform.scale, 15.0f);
            m_AtkFlag = true;
        }
    }
    //atk logic end
}

void Zombie::Attacked(int Damage, glm::vec2 Dir, float Velocity){
    if (GetState() == c_state::atk && m_player->GetState() != c_state::block){
        m_Hp -= Damage;
        return;
    }

    if (GetState() == c_state::atked){
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        if (temp == nullptr) return;
        if (temp->GetCurrentFrameIndex() >= temp->GetFrameCount()-1){
            temp->SetCurrentFrame(0);
            SetState(c_state::idle);
        }
        return;
    }
    LOG_DEBUG(m_Hp);

    if (IsContainState(c_state::atked)) SetState(c_state::atked);
    else InitState(c_state::atked, {6}, {RESOURCE_DIR"/Zombie/Atked/Atked_"});
    auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    temp->Play();
    m_Hp -= Damage;
    VelocityX += Velocity * ((Dir.x > 0) ? 1 : -1);
}

void Zombie::Move(float dt){
    if (GetState() == c_state::atk) return; //atk cannot move
    if (GetState() == c_state::atked) return;   //atked cannot move

    //trace player
    if (IsSameLevelNearBy(m_player,m_DetectRange)){  
        //c_state = move
        m_state = mob_state::trace;
        if (m_player->m_WorldPos.x > m_WorldPos.x + 10){   
            m_Transform.scale.x = 1.0f;
            //Add state
            if (!IsContainState(c_state::R_move)) InitState(c_state::R_move, {22}, {RESOURCE_DIR"/Zombie/move/move_"});
            else SetState(c_state::R_move);
        }
        else if (m_player->m_WorldPos.x < m_WorldPos.x - 10){
            m_Transform.scale.x = -1.0f;
            //Add state
            if (!IsContainState(c_state::L_move)) InitState(c_state::L_move, {22}, {RESOURCE_DIR"/Zombie/move/move_"});
            else SetState(c_state::L_move);
        }
    }
    else{   //遊蕩狀態
        Wander(RESOURCE_DIR"/Zombie/move/move_", 22);
    }
    //move
    switch (GetState()){
    case c_state::idle :
        VelocityX = 0;
        break;
    case c_state::L_move :
        VelocityX -= AccelerationX * dt;
        if (VelocityX < -1*MaxSpeed) VelocityX = -1 * MaxSpeed;
        break;
    case c_state::R_move :
        VelocityX += AccelerationX * dt;
        if (VelocityX > MaxSpeed) VelocityX = MaxSpeed;
        break;
    default:
        break;
    } 
}

void Zombie::Update(float dt){
    InGround = Physics::IsOnGround(m_WorldPos, m_World.SolidObjs, m_World.OneSidedPlatforms);
    Physics::ApplyGravity(VelocityY, InGround, Gravity, MaxFallSpeed);
    if (InGround) Physics::SlowDown(VelocityX, Friction);

    if (!m_AtkCoolDownTimer.IsTimeout()){
        if (GetState() == c_state::atked) Attacked(0, glm::vec2(0, 0), 0.0f);
        else SetState(c_state::idle);  // 在 cooldown 時完全 idle
        m_AtkFlag = false;        // 允許 cooldown 結束後重新攻擊
        PushPlayer();
        FixPos(dt);
        m_WorldPos.x += VelocityX * dt;
        m_WorldPos.y += VelocityY * dt;
        return;
    }

    Move(dt);
    
    // atk behavior
    if ((VelocityX > 0 && m_player->m_WorldPos.x > m_WorldPos.x && m_player->m_WorldPos.x < m_WorldPos.x + right) ||
        (VelocityX < 0 && m_player->m_WorldPos.x < m_WorldPos.x && m_player->m_WorldPos.x > m_WorldPos.x - left)){
    
        VelocityX = 0;
        
        //set atk state
        if (IsContainState(c_state::atk)) SetState(c_state::atk, {}, false);
        else InitState(c_state::atk, {28}, {RESOURCE_DIR"/Zombie/atk/atk_"});
        Attack(dt);
    }
    else if (GetState() == c_state::atk) Attack(dt);
    // atk end

    if (GetState() == c_state::atked) Attacked(0, glm::vec2(0, 0), 0.0f);
    
    PushPlayer();
    FixPos(dt);

    m_WorldPos.x += VelocityX * dt;
    m_WorldPos.y += VelocityY * dt;
}