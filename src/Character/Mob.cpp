#include "Character/Mob.hpp"

void Mob::Wander(const std::string& path, const size_t frames){
    if (m_state == mob_state::trace){
        m_state = mob_state::wander;
        SetState(c_state::idle);
        m_WanderTimer.SetTime(500, 1000);
        m_WanderTimer.ResetTime();
    }
    if (m_WanderTimer.IsTimeout()){ //update state
        c_state nextstate;
        switch (GetState()){
        case c_state::idle :
            nextstate = (std::rand() % 2 == 0) ? c_state::L_move : c_state::R_move;
            m_WanderTimer.SetTime(500, 1000);
            break;
        case c_state::L_move :
            nextstate = c_state::idle;
            m_WanderTimer.SetTime(2000, 5000);
            break;
        case c_state::R_move :
            nextstate = c_state::idle;
            m_WanderTimer.SetTime(2000, 5000);
            break;
        default:
            nextstate = c_state::idle;
            break;
        }
        //set state
        if (IsContainState(nextstate))SetState(nextstate);
        else InitState(nextstate, {frames}, {path});
        //turn
        if (GetState() == c_state::L_move) m_Transform.scale.x = -1;
        else if (GetState() == c_state::R_move) m_Transform.scale.x = 1;

        m_WanderTimer.ResetTime();
    }
}

void Mob::PushPlayer(){
    auto obj = std::static_pointer_cast<MapObj>(m_player);
    if (Collision::IsIntersectAABB(Collision::GetAABB(this), Collision::GetAABB(m_player.get())) ){
        m_player->Pushed();            
    }    
}