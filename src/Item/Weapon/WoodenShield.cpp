#include "Item/Weapon/WoodenShield.hpp"

#include "Character/Mob.hpp"

WoodenShield::WoodenShield() : Shield(RESOURCE_DIR"/Item/WoodenShield/Icon.png", "Wooden Shield") {
    m_BlockDrawable = std::make_shared<Util::Image>(RESOURCE_DIR"/Item/WoodenShield/Block_0.png");
    
    std::vector<std::string> paths;
    for (int i = 0; i < 9; i++){
        paths.push_back(RESOURCE_DIR"/Item/WoodenShield/Behavior/BlockEnd_" + std::to_string(i) + ".png");
    }
    m_BlockEndDrawable = std::make_shared<Util::Animation>(paths, true, 20, false);

    m_defense = 0.75f;
}



void WoodenShield::Parry(std::shared_ptr<Mob>& mob, const glm::vec2& Dir){
    mob->Attacked(ParryDamage, Dir, 10.0f);
    LOG_DEBUG("Parry");
    //WIP
}
