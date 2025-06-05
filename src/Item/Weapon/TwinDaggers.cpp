#include "Item/Weapon/TwinDaggers.hpp"
#include "Character/Mob.hpp"

TwinDaggers::TwinDaggers() : Weapon(RESOURCE_DIR"/Item/TwinDaggers/Icon.png", "this is a Weapon"){
    m_SegCount = 3;
    m_Player_Drawables.resize(m_SegCount);
    m_AtkPoint.resize(m_SegCount);
    m_AtkTimes.resize(m_SegCount);
    m_AtkableFrames.resize(m_SegCount);
    
    std::vector<std::string> path;
    for (int i = 0; i < 11; i++){
        path.push_back(RESOURCE_DIR"/Item/TwinDaggers/A/A_" + std::to_string(i) + ".png");
    }
    m_Player_Drawables[0] = std::make_shared<Util::Animation>(path, true, 30 ,false, 0);

    for(int i = 11; i < 14; i++){
        path.push_back(RESOURCE_DIR"/Item/TwinDaggers/A/A_" + std::to_string(i) + ".png");
    }
    m_Player_Drawables[2] = std::make_shared<Util::Animation>(path, true, 30, false, 0);

    path.clear();
    for (int i = 0; i < 16; i++){
        path.push_back(RESOURCE_DIR"/Item/TwinDaggers/B/B_" + std::to_string(i) + ".png");
    }
    m_Player_Drawables[1] = std::make_shared<Util::Animation>(path, true, 30, false, 0);

    m_AtkPoint[0] = 40, m_AtkPoint[1] = 55, m_AtkPoint[2] = 90;

    for(auto& temp : m_AtkTimes){temp = 1;}


    for (int i = 4; i < 11; i++){m_AtkableFrames[0].push_back(i);}
    for (int i = 4; i < 12; i++){m_AtkableFrames[1].push_back(i);}
    for (int i = 4; i < 11; i++){m_AtkableFrames[2].push_back(i);}
}

void TwinDaggers::Use(std::shared_ptr<Mob>& mob, const glm::vec2& Dir, int combo){
    mob->Attacked(m_AtkPoint[combo], Dir, 5.0f);
}

Rect TwinDaggers::GetHitBox(const glm::vec2& Pos, const glm::vec2& Dir, int combo){
    Rect result;
    result.x = Pos.x, result.y = Pos.y + 30;
    result.width = 60;
    result.height = 60;
    result.x += Dir.x > 0 ? 30 : -30;
    result.width *= 2, result.height *= 2;
    return result;
}