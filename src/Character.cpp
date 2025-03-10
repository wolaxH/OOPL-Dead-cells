#include "Character.hpp"


#include "Util/Logger.hpp"

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

bool Character::IsCollsion(std::shared_ptr<Util::GameObject> other){
    
    glm::vec2 Pos = m_Transform.translation;
    glm::vec2 other_Pos = other->GetTransform().translation;

    float other_distance_X = abs(other->GetScaledSize().x/ 2);
    float other_distance_Y = other->GetScaledSize().y/ 2;
    
    bool x = ((Pos.x < other_Pos.x + other_distance_X) && (Pos.x + 10 > other_Pos.x - other_distance_X)) ||
             ((Pos.x > other_Pos.x - other_distance_X) && (Pos.x - 10 < other_Pos.x + other_distance_X));

    bool y = ((Pos.y+60 < other_Pos.y + other_distance_Y) && (Pos.y+60 > other_Pos.y - other_distance_Y)) ||
             ((Pos.y > other_Pos.y - other_distance_Y) && (Pos.y < other_Pos.y + other_distance_Y));

    return x && y;
}


