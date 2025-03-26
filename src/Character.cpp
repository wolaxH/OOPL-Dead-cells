#include "Character.hpp"

#include "Util/Logger.hpp"

Character::Character(std::vector<std::string>& path, int Hp) : Hp(Hp){
    m_Drawable = std::make_shared<Util::Animation>(path, true, 20, true, 0);
    State = c_state::idle;
    D_Manager[State] = m_Drawable;
}


void Character::SetState(c_state State, std::vector<std::string> path, bool loop){
    try{
        m_Drawable = D_Manager.at(State);   //may throw error
        this->State = State;
    } catch (const std::out_of_range &e){
        if (!path.empty()){
            m_Drawable = std::make_shared<Util::Animation>(path, true, 20, loop, 0);
            D_Manager[State] = m_Drawable;
            this->State = State;
        }
    }
}

bool Character::IsCollsion(std::shared_ptr<MapObj> other){


    glm::vec2 Pos = m_WorldPos;
    glm::vec2 other_Pos = other->m_WorldPos;

    //glm::vec2 other_distance = other->GetScaledSize() / 2.0f;
    
    glm::vec2 other_scale = abs(other->GetTransform().scale);
    glm::vec2 scale = abs(GetTransform().scale); 
    int t_right = right * scale.x,  //temp borger
        t_left = left * scale.x,
        t_top = top * scale.y,
        t_bottom = bottom * scale.y;

    int other_right = other->right * other_scale.x,
        other_left = other->left * other_scale.x,
        other_top = other->top * other_scale.y,
        other_bottom = other->bottom * other_scale.y;



    bool x = ((Pos.x < other_Pos.x + other_right) && (Pos.x + t_right > other_Pos.x - other_left)) ||
             ((Pos.x > other_Pos.x - other_left) && (Pos.x - t_left < other_Pos.x + other_right));

    bool y = ((Pos.y < other_Pos.y + other_top) && (Pos.y + t_top > other_Pos.y - other_bottom)) ||
             ((Pos.y > other_Pos.y - other_bottom) && (Pos.y - t_bottom < other_Pos.y + other_top));
    /*
    bool x = ((Pos.x < other_Pos.x + other_distance.x) && (Pos.x + right > other_Pos.x - other_distance.x)) ||
             ((Pos.x > other_Pos.x - other_distance.x) && (Pos.x - left < other_Pos.x + other_distance.x));

    bool y = ((Pos.y < other_Pos.y + other_distance.y) && (Pos.y + top > other_Pos.y - other_distance.y)) ||
             ((Pos.y > other_Pos.y - other_distance.y) && (Pos.y - bottom < other_Pos.y + other_distance.y));
    */

    return x && y;
}

void Character::InitState(c_state state, const std::vector<std::size_t>& frames, const std::vector<std::string>& paths){

    std::vector<std::string> Img;
    std::vector<std::string> temp;

    for (std::size_t i =0; i < frames.size(); i++){
        temp.clear();
        for (std::size_t j = 0; j < frames[i]; j++){temp.push_back(paths[i] + std::to_string(j) + ".png");}
        Img.insert(Img.end(), temp.begin(), temp.end());
    }

    if (state == c_state::jump || state == c_state::fall || state == c_state::atk) SetState(state, Img, false);
    else SetState(state, Img);
}

void Character::applyGravity(){
    if (!InGround()){
        VelocityY -= Gravity;
        if (VelocityY < -1*MaxFallSpeed) VelocityY = -1*MaxFallSpeed;
    }
    else{
        if (VelocityY < 0) VelocityY = 0;
    }
}

void Character::FixPos(){
    int breakFlag = 0;
    
    for (auto&Solid : SolidObjs){
        breakFlag = 0;
        
        m_WorldPos.x += VelocityX;
        if (IsCollsion(Solid)){
            m_WorldPos.x -= VelocityX;
            VelocityX = 0;
            breakFlag++;
        }
        m_WorldPos.x -= VelocityX;
        
        m_WorldPos.y += VelocityY;
        if (IsCollsion(Solid)){
            m_WorldPos.y -= VelocityY;
            VelocityY = 0;
            breakFlag++;
        }
        m_WorldPos.y -= VelocityY;
        
        if (breakFlag == 2) break;
    }
}

bool Character::InGround(){

    glm::vec2 Pos = m_WorldPos;
    glm::vec2 other_Pos;

    glm::vec2 other_distance;
    float x, y;

    for (auto& Solid : SolidObjs){
        other_Pos = Solid->m_WorldPos;
        other_distance = Solid->GetScaledSize() / 2.0f;

        x = !((Pos.x < other_Pos.x - other_distance.x - 1) || (Pos.x > other_Pos.x + other_distance.x + 1));
        y = (Pos.y > other_Pos.y - other_distance.y) && (Pos.y < other_Pos.y + other_distance.y + 2);
        if (x && y) return true;
    }
    return false;
}

