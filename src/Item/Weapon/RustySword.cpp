#include "Item/Weapon/RustySword.hpp"
#include "Character/Mob.hpp"

RustySword::RustySword() : Weapon(RESOURCE_DIR"/Item/RustySword/Icon.png", 20, 50.0f, "Knife"){
    m_AtkPoint = 20;
    m_AtkRange = 70.0f; //temp
    
    std::vector<std::string> path;
    m_Player_Drawables.resize(3);

    for (int i = 0; i < 11; i++){
        path.push_back(RESOURCE_DIR"/Item/RustySword/A/A_" + std::to_string(i) + ".png");
    }
    m_Player_Drawables[0] = std::make_shared<Util::Animation>(path, true, 20 ,false, 0);
    m_Player_Drawables[2] = std::make_shared<Util::Animation>(path, true, 20 ,false, 0);

    path.clear();
    for (int i = 0; i < 12; i++){
        path.push_back(RESOURCE_DIR"/Item/RustySword/B/B_" + std::to_string(i) + ".png");
    }
    m_Player_Drawables[1] = std::make_shared<Util::Animation>(path, true, 20 ,false, 0);
}

void RustySword::Use(std::shared_ptr<Mob>& mob){
    mob->Attacked(123, glm::vec2(1,1));
}

Rect RustySword::GetHitBox(const glm::vec2& Pos, const glm::vec2& Dir){
    Rect result;
    result.x = Pos.x, result.y = Pos.y + 30;
    result.width = 30;
    result.height = 60;
    if (Dir.x > 0){ //右
        result.x += 20;
    }
    else{   //左
        result.x -= 20;
    }
    return result;
}