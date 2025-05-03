#include "Character/Zombie.hpp"

#include <random>

#include "Util/Logger.hpp"


void Zombie::Attack(float dt){  //player

    //rendering 
    auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    if (temp->GetState() != Util::Animation::State::PLAY){  // init atk behavior
        SetState(c_state::idle);
        AtkFlag = false;
        return;
    }
    //rendering end

    if (AtkFlag) return;    //already attacked
    if (temp->GetCurrentFrameIndex() >= temp->GetFrameCount()-4 ||  //atkable frame only 18 ~ 24
        temp->GetCurrentFrameIndex() < 17 ) return; 

    //the real atk logic
    if (m_Transform.scale.x > 0){   //atk forward right
        if (player->m_WorldPos.x > m_WorldPos.x && player->m_WorldPos.x < m_WorldPos.x + AtkRange){
            player->Attacked(AtkPoint);
            AtkFlag = true;
        }
    }
    else{
        if (player->m_WorldPos.x < m_WorldPos.x && player->m_WorldPos.x > m_WorldPos.x - AtkRange){
            player->Attacked(AtkPoint);
            AtkFlag = true;
            
        }
    }
    //atk logic end
}

bool Zombie::IsPlayerNearby(){
    glm::vec2 D = player->m_WorldPos - m_WorldPos;
    return glm::length(D) <= DetectRange;
}

void Zombie::Move(float dt){
    if (GetState() == c_state::atk) return; //atk cannot move

    //trace player
    if (IsPlayerNearby()){  
        //c_state = move
        m_state = mob_state::trace;
        if (player->m_WorldPos.x > m_WorldPos.x + 10){   
            m_Transform.scale.x = 1.0f;
            //Add state
            if (!IsContainState(c_state::R_move)) InitState(c_state::R_move, {22}, {RESOURCE_DIR"/Zombie/move/move_"});
            else SetState(c_state::R_move);
        }
        else if (player->m_WorldPos.x < m_WorldPos.x - 10){
            m_Transform.scale.x = -1.0f;
            //Add state
            if (!IsContainState(c_state::L_move)) InitState(c_state::L_move, {22}, {RESOURCE_DIR"/Zombie/move/move_"});
            else SetState(c_state::L_move);
        }
    }
    else{   //遊蕩狀態
        if (m_state == mob_state::trace){
            m_state = mob_state::wander;
            SetState(c_state::idle);
            timer.SetChangeTime(500, 1000);
            timer.ResetCheckTime();
        }
        if (timer.IsTimeout()){ //update state
            c_state nextstate;
            switch (GetState()){
            case c_state::idle :
                nextstate = (std::rand() % 2 == 0) ? c_state::L_move : c_state::R_move;
                timer.SetChangeTime(500, 1000);
                break;
            case c_state::L_move :
                nextstate = c_state::idle;
                timer.SetChangeTime(2000, 5000);
                break;
            case c_state::R_move :
                nextstate = c_state::idle;
                timer.SetChangeTime(2000, 5000);
                break;
            default:
                nextstate = c_state::idle;
                break;
            }
            //set state
            if (IsContainState(nextstate))SetState(nextstate);
            else InitState(nextstate, {22}, {RESOURCE_DIR"/Zombie/move/move_"});
            //turn
            if (GetState() == c_state::L_move) m_Transform.scale.x = -1;
            else if (GetState() == c_state::R_move) m_Transform.scale.x = 1;

            timer.ResetCheckTime();
        }
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
    Move(dt);
    applyGravity(dt);
    
    //atk behavior
    if ((VelocityX > 0 && player->m_WorldPos.x > m_WorldPos.x && player->m_WorldPos.x < m_WorldPos.x + right) ||
        (VelocityX < 0 && player->m_WorldPos.x < m_WorldPos.x && player->m_WorldPos.x > m_WorldPos.x - left)){
    
        VelocityX = 0;
        
        //set atk state
        if (IsContainState(c_state::atk)) SetState(c_state::atk, {}, false);
        else InitState(c_state::atk, {28}, {RESOURCE_DIR"/Zombie/atk/atk_"});
        Attack(dt);
    }
    else if (GetState() == c_state::atk) Attack(dt);
    //atk end
    
    PushPlayer();
    FixPos();

    m_WorldPos.x += VelocityX * dt;
    m_WorldPos.y += VelocityY * dt;
}