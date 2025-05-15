#include "Item/Weapon/Bone.hpp"
#include "Character/Mob.hpp"

Bone::Bone() : Weapon(RESOURCE_DIR"/Item/Bone/icon.png", "a Bone"){

    std::vector<std::string> paths;
    paths.resize(3);
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
        paths.push_back(RESOURCE_DIR"/Item/Bone/B/B_" + std::to_string(i) + ".png");
    }
    m_Player_Drawables[2] = std::make_shared<Util::Animation>(paths, true, 30 ,false, 0);

}


void Bone::Use(std::shared_ptr<Mob>& mob, const glm::vec2& Dir, int combo){
    mob->Attacked(m_AtkPoint[combo], Dir);
}

Rect Bone::GetHitBox(const glm::vec2& Pos, const glm::vec2& Dir){
    Rect result;
    result.x = Pos.x, result.y = Pos.y + 30;
    result.width = 60;
    result.height = 60;
    result.x += Dir.x > 0 ? 30 : -30;
    result.width *= 2, result.height *= 2;
    return result;
}