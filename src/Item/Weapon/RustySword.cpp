#include "Item/Weapon/RustySword.hpp"
#include "Character/Mob.hpp"

RustySword::RustySword() : Weapon(RESOURCE_DIR"/Item/RustySword/Icon.png", "this is a Weapon"){
    m_SegCount = 3;
    m_Player_Drawables.resize(m_SegCount);
    m_AtkPoint.resize(m_SegCount);
    m_AtkTimes.resize(m_SegCount);
    m_AtkableFrames.resize(m_SegCount);
    
    std::vector<std::string> path;
    for (int i = 0; i < 8; i++){
        path.push_back(RESOURCE_DIR"/Item/RustySword/A/A_" + std::to_string(i) + ".png");
    }
    m_Player_Drawables[0] = std::make_shared<Util::Animation>(path, true, 20 ,false, 0);
    for (int i = 8; i < 11; i++){
        path.push_back(RESOURCE_DIR"/Item/RustySword/A/A_" + std::to_string(i) + ".png");
    }
    m_Player_Drawables[2] = std::make_shared<Util::Animation>(path, true, 20 ,false, 0);

    path.clear();
    for (int i = 0; i < 12; i++){
        path.push_back(RESOURCE_DIR"/Item/RustySword/B/B_" + std::to_string(i) + ".png");
    }
    m_Player_Drawables[1] = std::make_shared<Util::Animation>(path, true, 20 ,false, 0);

    m_AtkPoint[0] = 25, m_AtkPoint[1] = 30, m_AtkPoint[2] = 40;

    for(auto& temp : m_AtkTimes){temp = 1;}

    for (int i = 4; i < 10; i++){m_AtkableFrames[0].push_back(i);}
    for (int i = 4; i < 10; i++){m_AtkableFrames[1].push_back(i);}
    for (int i = 6; i < 14; i++){m_AtkableFrames[2].push_back(i);}
}

void RustySword::Use(std::shared_ptr<Mob>& mob, const glm::vec2& Dir, int combo){
    mob->Attacked(m_AtkPoint[combo], Dir);
}

Rect RustySword::GetHitBox(const glm::vec2& Pos, const glm::vec2& Dir, int combo){
    Rect result;
    result.x = Pos.x, result.y = Pos.y + 30;
    result.width = 60;
    result.height = 60;
    result.x += Dir.x > 0 ? 30 : -30;
    result.width *= 2, result.height *= 2;
    return result;
}