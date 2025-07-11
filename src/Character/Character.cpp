#include "Character/Character.hpp"

#include "Util/Logger.hpp"

Character::Character(std::vector<std::string>& path, int Hp, GameWorldContext& World) 
    : m_Hp(Hp), m_World(World) {
    m_Drawable = std::make_shared<Util::Animation>(path, true, 30, true, 0);
    State = c_state::idle;
    D_Manager[State] = m_Drawable;
    m_ZIndex = 30;
}


void Character::SetState(c_state State, std::vector<std::string> path, bool Isloop){
    try{
        m_Drawable = D_Manager.at(State);   //may throw error
        this->State = State;
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        if (temp && !temp->GetLooping()) {
            temp->SetCurrentFrame(0);
            temp->Play();
        }
    } catch (const std::out_of_range &e){

        if (!path.empty()){
            m_Drawable = std::make_shared<Util::Animation>(path, true, 30, Isloop, 0);
            D_Manager[State] = m_Drawable;
            this->State = State;
        }
    }
}

void Character::InitState(c_state state, const std::vector<std::size_t>& frames, const std::vector<std::string>& paths){
    
    std::vector<std::string> Img;
    std::vector<std::string> temp;
    
    std::vector<c_state> NotLoopingState = {
        c_state::jump, c_state::fall, c_state::atk, c_state::clinb, 
        c_state::roll, c_state::crouch, c_state::atked, c_state::block, 
        c_state::heal, c_state::death};
    
    for (std::size_t i = 0; i < frames.size(); i++){
        temp.clear();
        for (std::size_t j = 0; j < frames[i]; j++){temp.push_back(paths[i] + std::to_string(j) + ".png");}
        Img.insert(Img.end(), temp.begin(), temp.end());
    }
    
    if (std::find(NotLoopingState.begin(), NotLoopingState.end(), state) != NotLoopingState.end()) SetState(state, Img, false);
    else SetState(state, Img);
}

void Character::InitState(c_state State, std::shared_ptr<Core::Drawable> drawable){
    if (IsContainState(State)) return;
    m_Drawable = drawable;
    D_Manager[State] = m_Drawable;
    this->State = State;
}

bool Character::IsNearBy(std::shared_ptr<MapObj> other, float distance){
    glm::vec2 D = m_WorldPos - other->m_WorldPos;
    return glm::length(D) <= distance;
}

bool Character::IsNearBy(glm::vec2 Point, float distance){
    glm::vec2 D = m_WorldPos - Point;
    return glm::length(D) <= distance;
}

void Character::ChangeDrawable(AccessKey , std::shared_ptr<Core::Drawable> PlayerDrawable, c_state state){
    auto PlayerAnim = std::dynamic_pointer_cast<Util::Animation>(PlayerDrawable);
    if (PlayerAnim){
        PlayerAnim->SetCurrentFrame(0);
        PlayerAnim->Play();
    }
    m_Drawable = PlayerDrawable;
    D_Manager[state] = m_Drawable;
}

void Character::FixPos(float dt){
    int breakFlag = 0;

    for (auto& Solid : m_World.SolidObjs) {
        if (!IsNearBy(Solid, 3000.0f)) continue;
        breakFlag = 0;

        // X 方向測試
        m_WorldPos.x += VelocityX * dt;
        if (Collision::IsIntersectAABB(this, Solid.get())) {
            m_WorldPos.x -= VelocityX * dt;
            VelocityX = 0;
            breakFlag++;
        }
        m_WorldPos.x -= VelocityX * dt;

        // Y 方向測試
        m_WorldPos.y += VelocityY * dt;
        if (Collision::IsIntersectAABB(this, Solid.get())) {
            m_WorldPos.y -= VelocityY * dt;
            VelocityY = 0;
            breakFlag++;
        }
        m_WorldPos.y -= VelocityY * dt;

        // 避免夾牆穿模
        if (breakFlag == 2) {
            m_WorldPos.y -= 1;
            return;
        }
    }

    //One-sided platform 處理
    for (auto& OSP : m_World.OneSidedPlatforms){
        if (m_WorldPos.y + top < OSP->m_WorldPos.y) continue;
        if (!IsNearBy(OSP, 640.0f)) continue;
        if (m_IgnoreOSP) return;

        //next frame Pos
        m_WorldPos.y += VelocityY * dt;
        auto aabb = Collision::GetAABB(this);
        auto OSP_aabb = Collision::GetAABB(OSP.get());

        float charCenter = aabb.left + (aabb.right - aabb.left) / 2;
        float ospLeft = OSP->m_WorldPos.x - OSP->left;
        float ospRight = OSP->m_WorldPos.x + OSP->right;

        // 只有在角色從上方下落且中心在平台上時才阻擋
        if (aabb.bottom < OSP_aabb.top - 1 &&
            !(charCenter < ospLeft - 1 || charCenter > ospRight + 1)) {
            m_WorldPos.y -= VelocityY * dt;
            VelocityY = 0;
            break;
        }
        m_WorldPos.y -= VelocityY * dt;
    }

}
