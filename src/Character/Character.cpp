#include "Character/Character.hpp"

#include "Util/Logger.hpp"

Character::Character(std::vector<std::string>& path, int Hp, 
    const std::vector<std::shared_ptr<SolidObj>>& SolidObjs, 
    const std::vector<std::shared_ptr<OneSidedPlatform>>& OSP) 
                : Hp(Hp)
                , r_SolidObjs(SolidObjs)
                ,r_OneSidedPlatforms(OSP) {
    m_Drawable = std::make_shared<Util::Animation>(path, true, 20, true, 0);
    State = c_state::idle;
    D_Manager[State] = m_Drawable;
}


void Character::SetState(c_state State, std::vector<std::string> path, bool Isloop){
    try{
        m_Drawable = D_Manager.at(State);   //may throw error
        this->State = State;
        if (!Isloop) {
            auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
            temp->SetCurrentFrame(0);
            temp->Play();
        }
    } catch (const std::out_of_range &e){
        if (!path.empty()){
            m_Drawable = std::make_shared<Util::Animation>(path, true, 20, Isloop, 0);
            D_Manager[State] = m_Drawable;
            this->State = State;
        }
    }
}

bool Character::IsCollsion(std::shared_ptr<MapObj> other){


    glm::vec2 Pos = m_WorldPos;
    glm::vec2 other_Pos = other->m_WorldPos;
    
    glm::vec2 other_scale = abs(other->GetTransform().scale);
    glm::vec2 scale = abs(GetTransform().scale); 
    float t_right = right * scale.x,  //temp borger
        t_left = left * scale.x,
        t_top = top * scale.y,
        t_bottom = bottom * scale.y;

    float other_right = other->right * other_scale.x,
        other_left = other->left * other_scale.x,
        other_top = other->top * other_scale.y,
        other_bottom = other->bottom * other_scale.y;



    bool x = ((Pos.x < other_Pos.x + other_right) && (Pos.x + t_right > other_Pos.x - other_left)) ||
             ((Pos.x > other_Pos.x - other_left) && (Pos.x - t_left < other_Pos.x + other_right));

    bool y = ((Pos.y < other_Pos.y + other_top) && (Pos.y + t_top > other_Pos.y - other_bottom)) ||
             ((Pos.y > other_Pos.y - other_bottom) && (Pos.y - t_bottom < other_Pos.y + other_top));

    return x && y;
}

void Character::InitState(c_state state, const std::vector<std::size_t>& frames, const std::vector<std::string>& paths){

    std::vector<std::string> Img;
    std::vector<std::string> temp;

    std::vector<c_state> NotLoopingState = {c_state::jump, c_state::fall, c_state::atk, c_state::clinb, c_state::roll};


    for (std::size_t i =0; i < frames.size(); i++){
        temp.clear();
        for (std::size_t j = 0; j < frames[i]; j++){temp.push_back(paths[i] + std::to_string(j) + ".png");}
        Img.insert(Img.end(), temp.begin(), temp.end());
    }

    if (std::find(NotLoopingState.begin(), NotLoopingState.end(), state) != NotLoopingState.end()) SetState(state, Img, false);
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

    /**
     * TODO:遍歷SolidObj 並使用舊邏輯修正
     *      遍歷OSP 並製作OSP的fix邏輯
     *      OSP邏輯: 玩家在OSP上
     */
    for (auto& Solid : r_SolidObjs){
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

        if (breakFlag == 2){
           m_WorldPos.y -= 1;
            return;
        }
    }
    
    for (auto& OSP : r_OneSidedPlatforms){
        if (m_WorldPos.y < OSP->m_WorldPos.y) continue;
        
        m_WorldPos.y += VelocityY;
        if (m_WorldPos.y < OSP->m_WorldPos.y && 
            !((m_WorldPos.x < OSP->m_WorldPos.x - OSP->GetScaledSize().x/2 - 1) || (m_WorldPos.x > OSP->m_WorldPos.x + OSP->GetScaledSize().x/2 + 1))){
            m_WorldPos.y -= VelocityY;
            VelocityY = 0;
            break;
        }
        m_WorldPos.y -= VelocityY;
    }
}

bool Character::InGround(){

    glm::vec2 Pos = m_WorldPos;
    glm::vec2 other_Pos;
    glm::vec2 other_scale;
    
    float x, y;

    /**
     * This also is a temporary solution, as the one sided platform is a solid object.
     */
    std::vector<std::shared_ptr<SolidObj>> r_temp;
    r_temp.reserve(r_SolidObjs.size() + r_OneSidedPlatforms.size());
    r_temp.insert(r_temp.end(), r_SolidObjs.begin(), r_SolidObjs.end());
    r_temp.insert(r_temp.end(), r_OneSidedPlatforms.begin(), r_OneSidedPlatforms.end());

    for (const auto& Solid : r_temp){
        other_Pos = Solid->m_WorldPos;
        other_scale = abs(Solid->GetScaledSize());

        x = !((Pos.x < other_Pos.x - other_scale.x/2 - 1) || (Pos.x > other_Pos.x + other_scale.x/2 + 1));
        y = (Pos.y > other_Pos.y - other_scale.y/2) && (Pos.y < other_Pos.y + other_scale.y/2 + 2);
        if (x && y) return true;
    }
    
    return false;
}

