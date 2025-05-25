#include "Item/Weapon/WoodenShield.hpp"

WoodenShield::WoodenShield() : Shield(RESOURCE_DIR"/Item/WoodenShield/Icon.png", "Wooden Shield") {
    m_BlockDrawable = std::make_shared<Util::Image>(RESOURCE_DIR"/Item/WoodenShield/Block_0.png");
    
    std::vector<std::string> paths;
    for (int i = 0; i < 9; i++){
        paths.push_back(RESOURCE_DIR"/Item/WoodenShield/Behavior/BlockEnd_" + std::to_string(i) + ".png");
    }
    m_BlockEndDrawable = std::make_shared<Util::Animation>(paths, true, 0, false);

    m_defense = 20;
}

void WoodenShield::Parry(std::shared_ptr<Mob>& mob, const glm::vec2& Dir, int combo){
    //WIP
}
