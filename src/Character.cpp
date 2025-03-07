#include "Character.hpp"

Character::Character(std::vector<std::string>& path, int Hp) : Hp(Hp){
    m_Drawable = std::make_shared<Util::Animation>(path, true, 20, true, 0);
    State = c_state::idle;
    D_Manager[State] = m_Drawable;
}


void Character::SetState(c_state State, std::vector<std::string> path){
    try{
        m_Drawable = D_Manager.at(State);   //may throw error
        this->State = State;
    } catch (const std::out_of_range &e){
        if (!path.empty()){
            m_Drawable = std::make_shared<Util::Animation>(path, true, 20, true, 0);
            D_Manager[State] = m_Drawable;
            this->State = State;
        }
    }
}

bool Character::IsCollsion(std::shared_ptr<GameObject> other){
    return false;
}
