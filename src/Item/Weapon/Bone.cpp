#include "Item/Weapon/Bone.hpp"
#include "Character/Mob.hpp"

Bone::Bone() : Weapon(RESOURCE_DIR"/Item/Bone/icon.png", "a Bone"){
    m_SegCount = 3;
    m_Player_Drawables.resize(m_SegCount);
    m_AtkPoint.resize(m_SegCount);
    m_AtkTimes.resize(3);
    m_AtkableFrames.resize(m_SegCount);

    std::vector<std::string> paths;
    for (int i = 0; i < 14; i++){
        paths.push_back(RESOURCE_DIR"/Item/Bone/A/A_" + std::to_string(i) + ".png");
    }
    m_Player_Drawables[0] = std::make_shared<Util::Animation>(paths, true, 30 ,false, 0);

    paths.clear();
    for (int i = 0; i < 18; i++){
        paths.push_back(RESOURCE_DIR"/Item/Bone/B/B_" + std::to_string(i) + ".png");
    }
    m_Player_Drawables[1] = std::make_shared<Util::Animation>(paths, true, 30 ,false, 0);

    paths.clear();
    paths.push_back(RESOURCE_DIR"/Item/Bone/C/Cstart_0.png");
    paths.push_back(RESOURCE_DIR"/Item/Bone/C/Cstart_1.png");
    for (int i = 0; i < 11; i++){
        paths.push_back(RESOURCE_DIR"/Item/Bone/C/C_" + std::to_string(i) + ".png");
    }
    std::vector<std::string>::iterator it = paths.begin();
    paths.insert(paths.end(), it+2, paths.end());
    m_Player_Drawables[2] = std::make_shared<Util::Animation>(paths, true, 30 ,false, 0);

    m_AtkPoint[0] = 50, m_AtkPoint[1] = 60, m_AtkPoint[2] = 40;

    m_AtkTimes[0] = 1, m_AtkTimes[1] = 1, m_AtkTimes[2] = 3;

    for (int i = 3; i < 10; i++){m_AtkableFrames[0].push_back(i);}
    for (int i = 7; i < 13; i++){m_AtkableFrames[1].push_back(i);}
    m_AtkableFrames[2] = {3, 7, 11, 15, 19, 23};
}


void Bone::Use(std::shared_ptr<Mob>& mob, const glm::vec2& Dir, int combo){
    mob->Attacked(m_AtkPoint[combo], Dir, 5.f);
}

Rect Bone::GetHitBox(const glm::vec2& Pos, const glm::vec2& Dir, int combo){
    Rect result;
    result.x = Pos.x, result.y = Pos.y + 30;
    if (combo < 2){
        result.width = 90;
        result.height = 60;
        result.x += Dir.x > 0 ? result.width/2 : -1* result.width/2;
    }
    else{
        result.width = 40;
        result.height = 60;    
    }
    result.width *= 2, result.height *= 2;
    return result;
}